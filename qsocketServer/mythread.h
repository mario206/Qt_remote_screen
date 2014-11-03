#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QTimer>
#include <QScreen>
#include <QPixmap>
#include <QGuiApplication>

class MyThread : public QThread
{

public:
    MyThread(qintptr);

public slots:
    void socketRead();
    void disconnected();
    void timeout();
    void sendScreen();

signals:

private:
    qintptr descriptor;
    QTcpSocket* socket;
    quint32 block_size;
    QTimer* t;
protected:
    void run();
    void stopSendScreen();
};

#endif // MYTHREAD_H
