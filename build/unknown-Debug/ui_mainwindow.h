/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *IP_AREA;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;
    QLineEdit *IPport;
    QLabel *label;
    QPushButton *testSockButton;
    QLineEdit *IPAdress;
    QLabel *label_2;
    QWidget *gridLayoutWidget_2;
    QGridLayout *TREE_AREA;
    QSpacerItem *horizontalSpacer;
    QListWidget *listWidget;
    QTreeWidget *DirTreeWidget;
    QLabel *FileLable;
    QLabel *DirLable;
    QPushButton *GetFileTree;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(778, 596);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(0, 310, 441, 141));
        IP_AREA = new QGridLayout(gridLayoutWidget);
        IP_AREA->setObjectName("IP_AREA");
        IP_AREA->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_2 = new QSpacerItem(20, 10000, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        IP_AREA->addItem(verticalSpacer_2, 2, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 10000, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        IP_AREA->addItem(verticalSpacer, 0, 0, 1, 1);

        IPport = new QLineEdit(gridLayoutWidget);
        IPport->setObjectName("IPport");

        IP_AREA->addWidget(IPport, 1, 3, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName("label");

        IP_AREA->addWidget(label, 1, 0, 1, 1);

        testSockButton = new QPushButton(gridLayoutWidget);
        testSockButton->setObjectName("testSockButton");

        IP_AREA->addWidget(testSockButton, 1, 4, 1, 1);

        IPAdress = new QLineEdit(gridLayoutWidget);
        IPAdress->setObjectName("IPAdress");

        IP_AREA->addWidget(IPAdress, 1, 1, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName("label_2");

        IP_AREA->addWidget(label_2, 1, 2, 1, 1);

        gridLayoutWidget_2 = new QWidget(centralwidget);
        gridLayoutWidget_2->setObjectName("gridLayoutWidget_2");
        gridLayoutWidget_2->setGeometry(QRect(0, 0, 441, 301));
        TREE_AREA = new QGridLayout(gridLayoutWidget_2);
        TREE_AREA->setObjectName("TREE_AREA");
        TREE_AREA->setHorizontalSpacing(5);
        TREE_AREA->setContentsMargins(5, 5, 5, 5);
        horizontalSpacer = new QSpacerItem(50, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        TREE_AREA->addItem(horizontalSpacer, 1, 1, 1, 1);

        listWidget = new QListWidget(gridLayoutWidget_2);
        listWidget->setObjectName("listWidget");
        listWidget->setMovement(QListView::Movement::Static);

        TREE_AREA->addWidget(listWidget, 1, 2, 1, 1);

        DirTreeWidget = new QTreeWidget(gridLayoutWidget_2);
        DirTreeWidget->headerItem()->setText(0, QString());
        DirTreeWidget->setObjectName("DirTreeWidget");

        TREE_AREA->addWidget(DirTreeWidget, 1, 0, 1, 1);

        FileLable = new QLabel(gridLayoutWidget_2);
        FileLable->setObjectName("FileLable");

        TREE_AREA->addWidget(FileLable, 0, 2, 1, 1);

        DirLable = new QLabel(gridLayoutWidget_2);
        DirLable->setObjectName("DirLable");

        TREE_AREA->addWidget(DirLable, 0, 0, 1, 1);

        GetFileTree = new QPushButton(centralwidget);
        GetFileTree->setObjectName("GetFileTree");
        GetFileTree->setGeometry(QRect(470, 50, 56, 18));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 778, 18));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        IPport->setText(QCoreApplication::translate("MainWindow", "9527", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\347\233\256\346\240\207IP\345\234\260\345\235\200\357\274\232", nullptr));
        testSockButton->setText(QCoreApplication::translate("MainWindow", "\346\265\213\350\257\225\351\223\276\346\216\245", nullptr));
        IPAdress->setText(QCoreApplication::translate("MainWindow", "127.0.0.1", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\347\253\257\345\217\243\345\217\267\357\274\232", nullptr));
        FileLable->setText(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\357\274\232", nullptr));
        DirLable->setText(QCoreApplication::translate("MainWindow", "\347\233\256\345\275\225\357\274\232", nullptr));
        GetFileTree->setText(QCoreApplication::translate("MainWindow", "\350\216\267\345\217\226", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
