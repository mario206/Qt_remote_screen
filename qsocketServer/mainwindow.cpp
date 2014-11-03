#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QPixmap>
#include <QTimer>
#include <QHostAddress>
#include <QImage>
#include <QDir>
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    init();
    label = new QLabel(this);
    label->setText(QString("Ip : %1\nPort : %2\n").arg(ipAddress,QString::number(port,10)));
    label->wordWrap();
    label->show();
    label->setGeometry(100,80,160,160);
    this->resize(320,320);

    connect(server,SIGNAL(newConnection()),this,SLOT(newsocket()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::socketSend()
{
//        t = new QTimer(this);
//        t->start(500);
//        connect(t,SIGNAL(timeout()),this,SLOT(timeout()));
    timeout();
}


void MainWindow::newsocket()
{
    qDebug() << "new Connection";
    socket = server->nextPendingConnection();
    connect(socket,SIGNAL(readyRead()),this,SLOT(socketRead()));
    connect(socket,SIGNAL(disconnected()),socket,SLOT(deleteLater()));

}

void MainWindow::socketRead()
{
    qDebug() << "Server socketRead";
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
    REQUEST request;
    in >> request;

    switch(request.requestType)
    {
    case REQUEST::StopSendScreen:
//        t->stop();
//        t->deleteLater();
        socket->disconnectFromHost();
        break;

    case REQUEST::sendScreen:
        socketSend();
        break;
    case REQUEST::leftClick:
        createMouseEvent(request.point,0);
        break;
    case REQUEST::rightClick:
        createMouseEvent(request.point,1);
         break;
    case REQUEST::doubleClick:
        createMouseEvent(request.point,2);

        break;
    }

    block_size = 0;
}

void MainWindow::createMouseEvent(QPoint p,quint32 type)
{
    qDebug() << "create mouse event" << p;
//    QMouseEvent *mpress;
//    QMouseEvent *mrelease;
//    mpress = new QMouseEvent(QEvent::MouseButtonDblClick,p,Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
////    mrelease = new QMouseEvent(t,p,Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
//    QApplication::sendEvent(this,mpress);
////    QApplication::sendEvent(QWidget::focusWidget(),mrelease);
    SetCursorPos(p.x(),p.y());
    switch (type)
    {
    case 0:
        mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
        mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
        break;
    case 1:
        mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
        mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
        break;
    case 2:
        mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
        mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
        mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
        mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
        break;

    }

}

void MainWindow::timeout()
{

    //构造文件名
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyyMMddhhmmss");
    QString picName = str + ".jpg";

    QString path;
    QDir dir;
    path=dir.currentPath();
    QString picPath =path + "/" + picName;

    QScreen *screen = QGuiApplication::primaryScreen();
    screen->grabWindow(0).save(picName);
    QPixmap pix;
    pix.load(path + "/" + picName);
    qDebug() << picPath;
//    QScreen *screen = QGuiApplication::primaryScreen();
//    QPixmap pix = screen->grabWindow(0);
    QByteArray block;  // 要发送的数据
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << (quint32)0; // 在头部预留2个字节
    out << pix; // 写入数据
    out.device()->seek(0);       // 回到数据头
    out << (quint32)(block.size() - sizeof(quint32)); // 写入数据大小
    socket->write(block);  // 发送数据
}

void MainWindow::init()
{
    block_size = 0;
    // 设置端口
    port = 8080;
    // 获取本机IP
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    server = new QTcpServer();
//    QHostAddress address(QString("127.0.0.1"));
    QHostAddress address(ipAddress);
    server->listen(address,port);


}
