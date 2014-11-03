#include "myscrollarea.h"
#include <QPicture>
MyScrollArea::MyScrollArea(QHostAddress m_ip, quint16 m_port)
    :ip(m_ip),port(m_port)
{
    block_size = 0;
    this->setWidget(&label);

    connectHost();
    connect(this,SIGNAL(quit()),this,SLOT(socketSend()));

}



void MyScrollArea::paintEvent(QPaintEvent *e)
{

    label.resize(this->size());
    QScrollArea::paintEvent(e);

}

MyScrollArea::~MyScrollArea()
{

}

bool MyScrollArea::close()
{

    emit quit();
    QScrollArea::close();
}

void MyScrollArea::mouseDoubleClickEvent(QMouseEvent *e)
{
    qDebug()<< "double Click" << e->pos() << e->type();
    REQUEST request(REQUEST::doubleClick,e->pos());
    sendChoice(request);
}

void MyScrollArea::mousePressEvent(QMouseEvent *e)
{
    qDebug()<< "mouse press" << e->pos();
    REQUEST request(REQUEST::leftClick,e->pos());
    if(e->button() == Qt::RightButton)
        request.requestType = REQUEST::rightClick;

    sendChoice(request);
}

void MyScrollArea::mouseReleaseEvent(QMouseEvent *e)
{

}

void MyScrollArea::socketRead()
{


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
    //数据接收完毕，读取
    qDebug() << "有新图像";
    QPixmap pix;
    in >> pix;
    label.setPixmap(pix);
    label.update();
    block_size = 0;
    this->resize(pix.size());
    REQUEST request(REQUEST::sendScreen);
    sendChoice(request);
    this->resize(pix.size());
//    QImage img;
//    in >> img;
//    label.setPixmap(QPixmap::fromImage(img));
//    label.update();
//    block_size = 0;
   // this->resize(img.size());


}

void MyScrollArea::socketSend()
{
    qDebug() << "妈蛋快停啊";
    socket->disconnectFromHost();
    socket->waitForDisconnected(3000);
    REQUEST request(REQUEST::StopSendScreen);
    sendChoice(request);
}

void MyScrollArea::connected()
{
    qDebug()<<"连接成功";
}



void MyScrollArea::connectHost()
{
    socket = new QTcpSocket(this);
    socket->connectToHost(ip,port);
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    REQUEST request(REQUEST::sendScreen);
    sendChoice(request);
    socket->waitForConnected(3000);
    connect(socket,SIGNAL(readyRead()),this,SLOT(socketRead()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(deleteLater()));

}

void MyScrollArea::sendChoice(REQUEST request)
{
    QByteArray block;  // 要发送的数据
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << (quint32)0; // 在头部预留2个字节
    out << request; // 写入数据
    out.device()->seek(0);       // 回到数据头
    out << (quint32)(block.size() - sizeof(quint32)); // 写入数据大小
    socket->write(block);
}


