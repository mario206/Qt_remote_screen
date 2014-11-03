#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QDataStream>
#include <QDebug>
#include <QScreen>
#include <QGridLayout>
#include <QScrollArea>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    initItem();
    // 初始化布局
    init();
    // 建立TCP
    connect(ui->get_btn,SIGNAL(clicked()),this,SLOT(GetData()));

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::GetData()
{
//   if(socket == 0 )
//       socket = new QTcpSocket();
//   block_size = 0;
//   socket->abort();
//   socket->connectToHost(ip,port);
//   area = new MyScrollArea();
//   area->show();
//   connect(area,SIGNAL(quit()),socket,SLOT(deleteLater()));
    area = new MyScrollArea(ip,port);
    area->show();

}

void MainWindow::init()
{

    port = 8080;

    ip   = QHostAddress("10.21.20.154");
    ui->setupUi(this);
}

void MainWindow::initItem()
{

}
//void MainWindow::ReadData()
//{
//    qDebug() << "有消息";
//    QDataStream in(socket);
//    in.setVersion(QDataStream::Qt_5_3);

//    // 第一次接受数据包
//    if(block_size == 0){
//        //检测有没数据头的大小
//        if(socket->bytesAvailable() < (int)sizeof(quint32))
//            return;
//        // 数据的大小
//        in >> block_size;
//    }
//    // 如果数据包不完整，返回,继续接受
//    if(socket->bytesAvailable() < block_size)
//        return;
//    // 数据接收完毕，读取
//    QPixmap pix;
//    in >> pix;
//    area->label.setPixmap(pix);

//    block_size = 0;

//}

