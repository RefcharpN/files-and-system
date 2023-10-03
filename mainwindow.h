#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <student.h>
#include <discipline.h>
#include <QElapsedTimer>
#include "QtGui/private/qzipreader_p.h"
#include "QtGui/private/qzipwriter_p.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void getWord(QString word);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_14_clicked();

private:
    Ui::MainWindow *ui;

    QList<Student> list;
    QList<Discipline> discipline_list;
    Discipline *discipline;

    QList<QString> files_paths;
    QString path_to_zip;

    QElapsedTimer threadTimer;

    int threadCount = 0;

};
#endif // MAINWINDOW_H
