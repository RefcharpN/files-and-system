#include "bfthread.h"
#include <QDebug>
#include <QCryptographicHash>



BfThread::BfThread(QString s, char startCh, char endCh): name(s), startCh(startCh), endCh(endCh)
{

}

void BfThread::run()
{
    qDebug() << "начало перебора";
    for (char c1 = startCh; c1 <= endCh; ++c1) {
        for (char c2 = 'a'; c2 <= 'z'; ++c2) {
            for (char c3 = 'a'; c3 <= 'z'; ++c3) {
                for (char c4 = 'a'; c4 <= 'z'; ++c4) {
                    for (char c5 = 'a'; c5 <= 'z'; ++c5) {
                        QString word = QString("%1%2%3%4%5").arg(c1).arg(c2).arg(c3).arg(c4).arg(c5);

                        if(QCryptographicHash::hash(word.toUtf8(), QCryptographicHash::Sha256).toHex() == "1115dd800feaacefdf481f1f9070374a2a81e27880f187396db67958b207cbad"
                            || QCryptographicHash::hash(word.toUtf8(), QCryptographicHash::Sha256).toHex() == "3a7bd3e2360a3d29eea436fcfb7e44c735d117c42d1c1835420b6b9942dd4f1b"
                            || QCryptographicHash::hash(word.toUtf8(), QCryptographicHash::Sha256).toHex() == "74e1bb62f8dabb8125a58852b63bdf6eaef667cb56ac7f7cdba6d7305c50a22f")
                        {
                            emit send(word);
                        }
                    }
                }
            }
        }
    }
    emit onFinish();
    qDebug() << "конец перебора";
}
