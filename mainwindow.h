#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<qtreewidget.h>
#include "cclientsocket.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    QString nIP{};
    QString nPort{};
    //Cclientsocket c;

signals:
    //ip，端口，命令，传输的数据，是否是信号包
    bool SendPacket(QString nIp,
                    QString nPort,
                    WORD cmd,
                    const QByteArray& payload=QByteArray() // Payload 默认为空
                    );//整合两个信号量
private slots:
    //void on_testSockButton_clicked();//测试链接
    void GetDirInfo();
    void handleItemExpanded(QTreeWidgetItem* item);

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
