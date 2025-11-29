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
    QObject::connect(this, &MainWindow::SendCmd,
                     &client, &Cclientsocket::startConnectionSlot);

    connect(ui->testSockButton, &QPushButton::clicked, // 1. 信号发送者和信号
            this,                                      // 2. 信号接收者 (当前对象)
            [this]() {                                 // 3. Lambda 函数体
            qDebug()<<"发出信号！";
                QString nIP=ui->IPAdress->text();
                QString nPort=ui->IPport->text();
                emit SendCmd(nIP,nPort,0);
            });

    connect(ui->GetFileTree,&QPushButton::clicked,this,[this](){
        QString nIP=ui->IPAdress->text();
        QString nPort=ui->IPport->text();
        SendCmd(nIP,nPort,1);
        Cclientsocket::GetInstance().DealCommand();
        //qDebug()<<"delend";
        QString data=Cclientsocket::GetInstance().GetPack().strData.data();
        //qDebug()<<data.toStdString();
        ui->DirTreeWidget->clear();
        //处理树
        for (int i = 0; i < data.length(); i++)
        {
            QChar driveChar = data.at(i);
            if (driveChar != QChar('\0') && driveChar != QChar(','))
            {
                QString driveName = QString(driveChar) + QString(":/");
                QTreeWidgetItem* newTopItem = new QTreeWidgetItem({driveName});
                ui->DirTreeWidget->addTopLevelItem(newTopItem);
            }
        }
        Cclientsocket::GetInstance().CloseSocket();
    });










}

MainWindow::~MainWindow()
{
    delete ui;
}

