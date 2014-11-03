#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QWidget>
#include <QPixmap>
#include <QVBoxLayout>
#include <myscrollarea.h>
class QLabel;
class QLineEdit;
class QPushButton;
class QGridLayout;
class QDialogButtonBox;
class QString;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
private slots:
    void GetData();
private:
    void init();
    void initItem();
    QTcpSocket* socket;
    quint32     block_size;
    QHostAddress ip;
    quint16     port;
    Ui::MainWindow *ui;
    MyScrollArea* area;





};

#endif // MAINWINDOW_H
