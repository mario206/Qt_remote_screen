#include "mythread.h"


MyThread::MyThread(qintptr m_descriptor)
{
    descriptor = m_descriptor;
    socket = new QTcpSocket();
    socket->setSocketDescriptor(descriptor);
    QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
}

void MyThread::socketRead()
{

}

void MyThread::run()
{


}


void MyThread::readyRead()
{
    qDebug() << "有消息";
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_3);
    // 第一次接受数据包
    if(block_size == 0){
        //检测有没数据头的大小
        if(socket->bytesAvailable() < (int)sizeof(quint32))
            return;
        // 数据的大小
        in >> block_size;
    }
    // 如果数据包不完整，返回,继续接受
    if(socket->bytesAvailable() < block_size)
        return;
    // 数据接收完毕，读取
    quint32 choice;
    in >> choice;
    block_size = 0;
    switch(choice)
    {
    case 0:
        stopSendScreen();
        break;
    case 1:
        sendScreen();
        break;
    }
}

void MyThread::disconnected()
{

}



void MyThread::timeout()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pix = screen->grabWindow(0);
    QByteArray block;  // 要发送的数据
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << (quint32)0; // 在头部预留2个字节
    out << pix; // 写入数据
    out.device()->seek(0);       // 回到数据头
    out << (quint32)(block.size() - sizeof(quint32)); // 写入数据大小
    socket->write(block);  // 发送数据
}

void MyThread::sendScreen()
{
    t = new QTimer(this);
    QObject::connect(t, SIGNAL(timeout()),this,SLOT(timeout()));
    QObject::connect(this,SIGNAL(stopSendScreen()),t,SLOT(stop()));
}

void MyThread::stopSendScreen()
{
    if(t != 0)
    {
        t->stop();
    }
}
