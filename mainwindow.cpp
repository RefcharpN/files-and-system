#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStorageInfo>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QThread>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QElapsedTimer>


#include "QtGui/private/qzipreader_p.h"
#include "QtGui/private/qzipwriter_p.h"

#include "bfthread.h"

#include <QCryptographicHash>
#include <QObject>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady()) {
            if (!storage.isReadOnly()) {
                this->ui->listWidget->addItem("раздел: " + storage.device() + "\n"
                                              + "name: " + storage.displayName() + "\n"
                                              + "size: " + QString("%1").arg(storage.bytesTotal()/1000/1000) + " МБ\n"
                                              + "avaliable size: " + QString("%1").arg(storage.bytesAvailable()/1000/1000) + " МБ\n"
                                              + "fileSystemType: " +storage.fileSystemType() + "\n");

            }
        }

    }


}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getWord(QString word)
{
    this->ui->label_19->setText(this->ui->label_19->text()+ QCryptographicHash::hash(word.toUtf8(), QCryptographicHash::Sha256).toHex() + " - " + word + "\n");

    this->threadCount++;
    if(this->threadCount == 3)
    {
        this->ui->label_19->setText(this->ui->label_19->text() + QString("время выполения - %1 секунд\n").arg(this->threadTimer.elapsed()/1000));
        this->threadCount = 0;
    }
}

void MainWindow::getWordMono(QString word)
{
    this->ui->label_14->setText(this->ui->label_14->text()+ QCryptographicHash::hash(word.toUtf8(), QCryptographicHash::Sha256).toHex() + " - " + word + "\n");

    this->threadCount++;
    if(this->threadCount == 3)
    {
        this->ui->label_14->setText(this->ui->label_14->text() + QString("время выполения - %1 секунд\n").arg(this->threadTimer.elapsed()/1000));
    }
}


void MainWindow::on_pushButton_clicked()//сохранить
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("сохранить"), "~/", tr("txt files (*.txt)"));

    QFile file(fileName);

    if(!file.open(QIODevice::ReadWrite)) {
        QMessageBox::information(0, "error", file.errorString());
        return;
    }


    QTextStream stream(&file);
    stream << this->ui->plainTextEdit->toPlainText()<< Qt::endl;
    stream.flush();

    file.close();
}


void MainWindow::on_pushButton_2_clicked()//открыть
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("открыть"), "~/", tr("text files (*.txt)"));

    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine() + "\n";

        this->ui->plainTextEdit->insertPlainText (line);
    }

    file.close();

}


void MainWindow::on_pushButton_3_clicked()//удалить
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("открыть"), "~/", tr("text files (*.txt)"));
    QFile file (fileName);
    file.remove();
}


void MainWindow::on_pushButton_5_clicked()//json открыть
{
    this->ui->listWidget_2->clear();
    list.clear();

    QString fileName = QFileDialog::getOpenFileName(this,tr("открыть"), "~/", tr("json files (*.json)"));

    QString val;

    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonParseError error;

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8(), &error);
    if (doc.isObject())
    {
        QJsonObject json = doc.object();
        QJsonArray jsonArray = json["students"].toArray();
        foreach (const QJsonValue & value, jsonArray)
        {
            if (value.isObject())
            {
                QJsonObject obj = value.toObject();
                QString fio = obj["name"].toString();
                QString year = obj["year"].toString();
                QString track = obj["track"].toString();

                list.append(Student(fio,year,track));
                this->ui->listWidget_2->addItem("ФИО: " + fio + "\nГод поступления: " + year+ "\nНаправление: " + track+ "\n");
            }
        }
    }


}


void MainWindow::on_pushButton_6_clicked()//json добавить
{
    list.append(Student(this->ui->lineEdit->text(),this->ui->lineEdit_2->text(),this->ui->lineEdit_3->text()));
    this->ui->listWidget_2->addItem("ФИО: " + this->ui->lineEdit->text() + "\nГод поступления: " + this->ui->lineEdit_2->text()+ "\nНаправление: " + this->ui->lineEdit_3->text()+ "\n");
}


void MainWindow::on_pushButton_4_clicked()//json сохранить
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("сохранить"), "~/", tr("json files (*.json)"));


    QJsonArray plot_array;
    for(auto item : list)
    {
        QJsonObject item_data;

        item_data.insert("name", QJsonValue(item.getFio()));
        item_data.insert("year", QJsonValue(item.getYear()));
        item_data.insert("track", QJsonValue(item.getTrack()));

        plot_array.push_back(QJsonValue(item_data));
    }

    QJsonObject final_object;

    final_object.insert(QString("students"), QJsonValue(plot_array));
    QJsonDocument doc(final_object);
    QString jsonString = doc.toJson(QJsonDocument::Indented);


    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream( &file );
    stream << jsonString;
    stream.flush();
}


void MainWindow::on_pushButton_7_clicked()//xml добавить
{
    discipline_list.append(Discipline(this->ui->lineEdit_4->text(),this->ui->lineEdit_5->text(),this->ui->lineEdit_6->text()));
    this->ui->listWidget_3->addItem("название: " + this->ui->lineEdit_4->text() + "\nГод утверждения: " + this->ui->lineEdit_5->text()
                                    + "\nПреподаватель: " + this->ui->lineEdit_6->text()+ "\n");
}


void MainWindow::on_pushButton_9_clicked()//xml сохранить
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("сохранить"), "~/", tr("xml files (*.xml)"));

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("disciplins");
    for(auto item : discipline_list)
    {
        xmlWriter.writeStartElement("disciplin");

            xmlWriter.writeStartElement("title");
                xmlWriter.writeCharacters(item.getTitle());
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("year");
                xmlWriter.writeCharacters(item.getYear());
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("teacher");
                xmlWriter.writeCharacters(item.getTeacher());
            xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();
    }



    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();


}


void MainWindow::on_pushButton_8_clicked()//xml открыть
{
    this->ui->listWidget_3->clear();
    discipline_list.clear();

    QString fileName = QFileDialog::getOpenFileName(this,tr("открыть"), "~/", tr("xml files (*.xml)"));

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,
                             "Ошибка файла",
                             "Не удалось открыть файл",
                             QMessageBox::Ok);
    } else
    {
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&file);
        xmlReader.readNext();
        while(!xmlReader.atEnd())
        {


            if(xmlReader.isStartElement())
            {
                if (xmlReader.name() == "title")
                {
                    this->discipline = new class Discipline();

                    discipline->setTitle( xmlReader.readElementText());
                } else if (xmlReader.name() == "year")
                {
                    discipline->setYear(xmlReader.readElementText());
                }else if (xmlReader.name() == "teacher")
                {
                    discipline->setTeacher(xmlReader.readElementText());

                    this->ui->listWidget_3->addItem("Название: " + discipline->getTitle() + "\nГод утверждения: " + discipline->getYear()+ "\nПреподаватель: "
                                                    + discipline->getTeacher()+ "\n");
                    discipline_list.append(Discipline(discipline->getTitle(),discipline->getYear(),discipline->getTeacher()));

                    discipline = NULL;
                    delete discipline;
                }
            }

            xmlReader.readNext();
        }
        file.close();

    }
}


void MainWindow::on_pushButton_12_clicked()//добавить файл
{
    this->path_to_zip = "";
    this->ui->pushButton_11->setEnabled(false);
    this->ui->pushButton_13->setEnabled(true);

    QString fileName = QFileDialog::getOpenFileName(this,tr("открыть"), "~/");

    this->files_paths.append(fileName);

    this->ui->listWidget_4->addItem(QString("Файл: %1\nПуть: %2\nРазмер: %3").arg(QFileInfo(fileName).fileName()).arg(fileName).arg(QFileInfo(fileName).size()));



}


void MainWindow::on_pushButton_10_clicked()//открыть архив
{
    this->ui->listWidget_4->clear();
    this->files_paths.clear();
    this->ui->pushButton_11->setEnabled(true);
    this->ui->pushButton_13->setEnabled(false);

    QString fileName = QFileDialog::getOpenFileName(this,tr("открыть"), "~/", tr("zip files (*.zip)"));

    QZipReader zip_reader(fileName);

    if (zip_reader.exists())
    {
        this->path_to_zip = fileName;
        QString zipInfo = "";
        zipInfo += "количество файлов в архиве = " + QString("%1\n").arg(zip_reader.count());
        foreach (QZipReader::FileInfo info, zip_reader.fileInfoList()) {
            if(info.isFile)
                zipInfo += "файл :" + QString("%1 %2 байт\n").arg(info.filePath).arg(info.size);
        }

        this->ui->listWidget_4->addItem(zipInfo);
        this->ui->pushButton_11->setEnabled(true);

    }
}


void MainWindow::on_pushButton_11_clicked()//разархивировать
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    QZipReader zip_reader(this->path_to_zip);

    zip_reader.extractAll(dir);
}


void MainWindow::on_pushButton_13_clicked()//заархивировать
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("сохранить"), "~/", tr("zip files (*.zip)"));


    QZipWriter zip(fileName);

    if (zip.status() != QZipWriter::NoError)
        return;

    zip.setCompressionPolicy(QZipWriter::AutoCompress);

    for(auto item : files_paths)
    {
        QString file_name = QFileInfo(item).fileName();
        QFile file(item);
        if (!file.open(QIODevice::ReadOnly))
            return;
        zip.setCreationPermissions(QFile::permissions(item));
        zip.addFile(file_name, file.readAll());
    }

    zip.close();
}


void MainWindow::on_pushButton_15_clicked()
{

    this->ui->label_19->clear();

    int threadCount = this->ui->comboBox->currentText().toInt();
    int alphPart = 26/threadCount;
    QVector<BfThread*> tasks;

    for(int i = 0; i < 26; i = i + alphPart)
    {
        int start = i;
        int end = i + alphPart - 1;

        if(end > 25)
        {
            end = 25;
        }

        BfThread thread = BfThread("test", char('a' + start), char('a' + end));
        tasks.append(new BfThread("test", char('a' + start), char('a' + end)));
    }

    for(int i = 0; i < tasks.length(); i++)
    {
        tasks[i]->start();

        QObject::connect(tasks.at(i), &BfThread::send, this, &MainWindow::getWord);
    }

    this->threadTimer.start();

}


void MainWindow::on_pushButton_14_clicked()
{
    this->ui->label_14->clear();
    BfThread* thread = new BfThread("test", 'a', 'z');
    QObject::connect(thread, &BfThread::send, this, &MainWindow::getWordMono);
    thread->start();
    this->threadTimer.start();
}

//void MainWindow::monoBrute()
//{
//    QElapsedTimer timer;
//    timer.start();
//    qDebug() << "начало перебора";
//    for (char c1 = 'a'; c1 <= 'z'; ++c1) {
//        for (char c2 = 'a'; c2 <= 'z'; ++c2) {
//            for (char c3 = 'a'; c3 <= 'z'; ++c3) {
//                for (char c4 = 'a'; c4 <= 'z'; ++c4) {
//                    for (char c5 = 'a'; c5 <= 'z'; ++c5) {
//                        QString word = QString("%1%2%3%4%5").arg(c1).arg(c2).arg(c3).arg(c4).arg(c5);

//                        if(QCryptographicHash::hash(word.toUtf8(), QCryptographicHash::Sha256).toHex() == "1115dd800feaacefdf481f1f9070374a2a81e27880f187396db67958b207cbad"
//                            || QCryptographicHash::hash(word.toUtf8(), QCryptographicHash::Sha256).toHex() == "3a7bd3e2360a3d29eea436fcfb7e44c735d117c42d1c1835420b6b9942dd4f1b"
//                            || QCryptographicHash::hash(word.toUtf8(), QCryptographicHash::Sha256).toHex() == "74e1bb62f8dabb8125a58852b63bdf6eaef667cb56ac7f7cdba6d7305c50a22f")
//                        {
//                            this->ui->label_14->setText(this->ui->label_14->text() + QCryptographicHash::hash(word.toUtf8(), QCryptographicHash::Sha256).toHex() + " - " + word + "\n");
//                        }
//                    }
//                }
//            }
//        }
//    }
//    qDebug() << "конец перебора";
//    this->ui->label_14->setText(this->ui->label_14->text() + QString("время выполения - %1 секунд\n").arg(timer.elapsed()/1000));
//}

