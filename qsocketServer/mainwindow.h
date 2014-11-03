#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QLabel>
#include <mythread.h>
#include <QMouseEvent>
#include <QEvent>
#include <QPoint>
#include <windows.h>
struct REQUEST
{
    enum ClientRequest {sendScreen,StopSendScreen,doubleClick,leftClick,rightClick};
    ClientRequest requestType;
    QPoint point;
    REQUEST(ClientRequest r,QPoint p = QPoint(0,0)):requestType(r),point(p){}
    REQUEST(){};
};
inline QDataStream& operator <<(QDataStream& out,const REQUEST& r)
{
    out << (quint32)r.requestType;
    out << r.point;
    return out;
}
inline QDataStream& operator >>(QDataStream& out, REQUEST& r)
{
    quint32 i;
    out >> i;
    r.requestType = (REQUEST::ClientRequest)i;
    out >> r.point;
    return out;
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void socketSend();
    void newsocket();
    void socketRead();
    void createMouseEvent(QPoint,quint32);
    void timeout();

private:
    void init();
    Ui::MainWindow *ui;
    QTcpServer* server;
    QTcpSocket* socket;
    QLabel* label;
    QTimer *t;
    QString ipAddress;
    quint16 port;
    quint32 block_size;
    enum state{sendScreen,stopSendScreen};

};

#endif // MAINWINDOW_H
