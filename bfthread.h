#ifndef BFTHREAD_H
#define BFTHREAD_H

#include <QThread>
#include <QString>


class BfThread: public QThread
{
    Q_OBJECT
public:
    explicit BfThread(QString s, char startCh, char endCh);

    void run();

signals:
    void send(QString word);
    void onFinish();

private:
    QString name;
    char startCh;
    char endCh;

};

#endif // BFTHREAD_H
