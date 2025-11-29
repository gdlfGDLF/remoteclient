#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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


signals:
    void SendCmd(QString nIP,QString nPort,int cmd);
private slots:
    //void on_testSockButton_clicked();//测试链接
private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
