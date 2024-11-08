/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "customdrawwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    CustomDrawWidget *widget;
    QTextBrowser *textBrowser;
    QRadioButton *red_Button;
    QRadioButton *green_Button;
    QPushButton *pushButton_deletePoints;
    QRadioButton *task1;
    QRadioButton *task3;
    QRadioButton *task2;
    QRadioButton *task4;
    QRadioButton *task5;
    QRadioButton *task6;
    QPushButton *getResult;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(982, 691);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        widget = new CustomDrawWidget(centralWidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(70, 10, 861, 341));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(70, 370, 861, 192));
        red_Button = new QRadioButton(centralWidget);
        red_Button->setObjectName("red_Button");
        red_Button->setGeometry(QRect(140, 570, 95, 20));
        green_Button = new QRadioButton(centralWidget);
        green_Button->setObjectName("green_Button");
        green_Button->setGeometry(QRect(260, 570, 95, 20));
        pushButton_deletePoints = new QPushButton(centralWidget);
        pushButton_deletePoints->setObjectName("pushButton_deletePoints");
        pushButton_deletePoints->setGeometry(QRect(350, 560, 121, 28));
        task1 = new QRadioButton(centralWidget);
        task1->setObjectName("task1");
        task1->setGeometry(QRect(480, 570, 171, 20));
        task3 = new QRadioButton(centralWidget);
        task3->setObjectName("task3");
        task3->setGeometry(QRect(800, 570, 151, 20));
        task2 = new QRadioButton(centralWidget);
        task2->setObjectName("task2");
        task2->setGeometry(QRect(650, 570, 141, 20));
        task4 = new QRadioButton(centralWidget);
        task4->setObjectName("task4");
        task4->setGeometry(QRect(480, 600, 95, 20));
        task5 = new QRadioButton(centralWidget);
        task5->setObjectName("task5");
        task5->setGeometry(QRect(560, 600, 201, 20));
        task6 = new QRadioButton(centralWidget);
        task6->setObjectName("task6");
        task6->setGeometry(QRect(780, 600, 151, 20));
        getResult = new QPushButton(centralWidget);
        getResult->setObjectName("getResult");
        getResult->setGeometry(QRect(350, 590, 121, 28));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 982, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName("mainToolBar");
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        red_Button->setText(QCoreApplication::translate("MainWindow", "Red", nullptr));
        green_Button->setText(QCoreApplication::translate("MainWindow", "Green", nullptr));
        pushButton_deletePoints->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \321\202\320\276\321\207\320\272\320\270", nullptr));
        task1->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\200\320\265\321\201\320\265\321\207\320\265\320\275\320\270\320\265 \320\276\321\202\321\200\320\265\320\267\320\272\320\276\320\262", nullptr));
        task3->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\277\321\203\320\272\320\273\320\260\321\217 \320\276\320\261\320\276\320\273\320\276\321\207\320\272\320\260", nullptr));
        task2->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\265\320\264\320\270\320\272\320\260\321\202 \320\277\320\276\320\262\320\276\321\200\320\276\321\202\320\260", nullptr));
        task4->setText(QCoreApplication::translate("MainWindow", "\320\224\320\265\320\273\320\276\320\275\320\265", nullptr));
        task5->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\200\320\265\321\201\320\265\321\207\320\265\320\275\320\270\321\217 \320\277\320\276\320\273\320\270\320\263\320\276\320\275\320\276\320\262 \320\270 \321\202\320\264", nullptr));
        task6->setText(QCoreApplication::translate("MainWindow", "\320\233\320\276\320\272\320\260\320\273\320\270\320\267\320\260\321\206\320\270\321\217 \321\202\320\276\321\207\320\272\320\270", nullptr));
        getResult->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\321\201\321\207\320\270\321\202\320\260\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
