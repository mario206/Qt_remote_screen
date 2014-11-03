#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = 0);

signals:

public slots:
    void socketRead();
private:
    enum state{send,stop};
    quint32 blocksize;
    quint32 choice;

};

#endif // MYTCPSOCKET_H
