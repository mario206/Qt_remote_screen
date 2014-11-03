#include "mytcpsocket.h"

MyTcpSocket::MyTcpSocket(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(socketRead()));

}

void MyTcpSocket::socketRead()
{

}
