#ifndef MYSCROLLAREA_H
#define MYSCROLLAREA_H

#include <QScrollArea>
#include <QLabel>
#include <QHostAddress>
#include <QTcpSocket>
#include <QMouseEvent>
#include <QPoint>

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

class MyScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit MyScrollArea(QHostAddress,quint16);
    void paintEvent(QPaintEvent*);
    ~MyScrollArea();
    bool close();
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
signals:
    void quit();
public slots:
    void socketRead();
    void socketSend();
    void connected();

private:
    QLabel label;
    QHostAddress ip;
    quint16      port;
    quint32  block_size;
    QTcpSocket* socket;
    void connectHost();
    void sendChoice(REQUEST);
};

#endif // MYSCROLLAREA_H
