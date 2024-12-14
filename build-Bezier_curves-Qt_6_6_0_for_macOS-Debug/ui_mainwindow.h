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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include "customdrawwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    CustomDrawWidget *widget;
    QCheckBox *Linear_curve;
    QCheckBox *Cubiccurve;
    QCheckBox *Qadratic_curve;
    QCheckBox *Manual_curve;
    QTextEdit *Pow_of_curve;
    QLabel *label;
    QPushButton *Draw_curve;
    QPushButton *Deletepoints;
    QTextBrowser *textBrowser;
    QCheckBox *numP_curve;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1114, 716);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        widget = new CustomDrawWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(0, 0, 1111, 531));
        Linear_curve = new QCheckBox(centralwidget);
        Linear_curve->setObjectName("Linear_curve");
        Linear_curve->setGeometry(QRect(0, 540, 101, 21));
        Cubiccurve = new QCheckBox(centralwidget);
        Cubiccurve->setObjectName("Cubiccurve");
        Cubiccurve->setGeometry(QRect(0, 580, 111, 20));
        Qadratic_curve = new QCheckBox(centralwidget);
        Qadratic_curve->setObjectName("Qadratic_curve");
        Qadratic_curve->setGeometry(QRect(0, 560, 121, 20));
        Manual_curve = new QCheckBox(centralwidget);
        Manual_curve->setObjectName("Manual_curve");
        Manual_curve->setGeometry(QRect(0, 600, 141, 20));
        Pow_of_curve = new QTextEdit(centralwidget);
        Pow_of_curve->setObjectName("Pow_of_curve");
        Pow_of_curve->setGeometry(QRect(270, 620, 131, 21));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 620, 291, 20));
        Draw_curve = new QPushButton(centralwidget);
        Draw_curve->setObjectName("Draw_curve");
        Draw_curve->setGeometry(QRect(430, 540, 231, 32));
        Deletepoints = new QPushButton(centralwidget);
        Deletepoints->setObjectName("Deletepoints");
        Deletepoints->setGeometry(QRect(430, 580, 231, 32));
        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(670, 540, 441, 111));
        numP_curve = new QCheckBox(centralwidget);
        numP_curve->setObjectName("numP_curve");
        numP_curve->setGeometry(QRect(210, 560, 141, 21));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1114, 37));
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
        Linear_curve->setText(QCoreApplication::translate("MainWindow", "Linear_curve", nullptr));
        Cubiccurve->setText(QCoreApplication::translate("MainWindow", "Cubic_curve", nullptr));
        Qadratic_curve->setText(QCoreApplication::translate("MainWindow", "Qadratic_curve", nullptr));
        Manual_curve->setText(QCoreApplication::translate("MainWindow", "Hight-pow_curve", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Put your Bezier curve pow here(default=4)", nullptr));
        Draw_curve->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\200\320\270\321\201\320\276\320\262\320\260\321\202\321\214 \320\272\321\200\320\270\320\262\321\213\320\265", nullptr));
        Deletepoints->setText(QCoreApplication::translate("MainWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214 \320\277\320\276\320\273\320\265", nullptr));
        numP_curve->setText(QCoreApplication::translate("MainWindow", "NumP-pow_curve", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
