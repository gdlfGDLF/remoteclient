#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cclientsocket.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //初始化套接字
    Cclientsocket& client=Cclientsocket::GetInstance();
    QObject::connect(this, &MainWindow::testLink,
                     &client, &Cclientsocket::startConnectionSlot);
    connect(ui->testSockButton, &QPushButton::clicked, // 1. 信号发送者和信号
            this,                                      // 2. 信号接收者 (当前对象)
            [this]() {                                 // 3. Lambda 函数体
                // 确保 testLink() 在 MainWindow 的 signals: 区域被声明
                qDebug()<<"发出信号！";
                emit testLink();
            });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_testSockButton_clicked()
{

}

