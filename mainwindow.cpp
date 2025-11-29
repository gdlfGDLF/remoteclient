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

    QObject::connect(this, &MainWindow::SendPacket,
                     &client, &Cclientsocket::startConnectionSlot);



    connect(ui->testSockButton, &QPushButton::clicked, // 1. 信号发送者和信号
            this,                                      // 2. 信号接收者 (当前对象)
            [this]() {                                 // 3. Lambda 函数体
            qDebug()<<"发出信号！";
                nIP=ui->IPAdress->text();
                nPort=ui->IPport->text();
                emit SendPacket(nIP,nPort,0);
            });
    //封装一下函数
    //Nip 和NPORT设置为类成员变量 省的每次都要分配
    QObject::connect(ui->GetFileTree, &QPushButton::clicked,
                     this,
                     &MainWindow::GetDirInfo);
    //点击展开子节点
    connect(ui->DirTreeWidget, &QTreeWidget::itemExpanded,
            this, &MainWindow::handleItemExpanded);

    //文件传输逻辑
    //发送命令->服务器->遍历盘符及其文件夹->打包发送给客户端->显示
    //发送命令 完成
    //服务器的浏览盘符完成
    //首先我要给这个treewidget添加一个信号 点击之后展开子节点
    //子节点从何而来 服务器返回搜索信息
    //

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::GetDirInfo()
{
    nIP=ui->IPAdress->text();
    nPort=ui->IPport->text();
    SendPacket(nIP,nPort,1);
    qDebug()<<"dealing";
    Cclientsocket::GetInstance().DealCommand();
    qDebug()<<"delend";
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
            //做一步储存 为了和服务器交互获取盘符下的文件信息
            newTopItem->setData(0, Qt::UserRole, driveName);
            newTopItem->addChild(new QTreeWidgetItem(QStringList("loading...")));

            ui->DirTreeWidget->addTopLevelItem(newTopItem);
        }
    }
    Cclientsocket::GetInstance().CloseSocket();
}
void MainWindow::handleItemExpanded(QTreeWidgetItem* item)
{
    qDebug()<<"handleItemExpanded";
    QString fullPath = item->data(0, Qt::UserRole).toString();
    if (fullPath.endsWith('/')) {
        // 移除末尾的斜杠，转换为系统期望的 "C:" 或 "C:/Path"
        fullPath.chop(1);
    }
    if (fullPath.isEmpty()) {
        fullPath = item->text(0); // 例如 "C:/" 优化方案：只传输c: 少传一个/
    }
    if (item->childCount() == 1 && item->child(0)->text(0) == "loading...") {
        item->removeChild(item->child(0));
        qDebug() << "Requesting directory listing for:" << fullPath;
        SendPacket(nIP,nPort,2,fullPath.toUtf8());
        // 后续的 UI 更新将在 MyClient::readData 槽函数中处理服务器的响应包。
    }
}
