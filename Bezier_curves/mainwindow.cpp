#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "bezierutils.h"

#include "longdouble.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->widget, &CustomDrawWidget::dataChanged,this,&MainWindow::getMouseCoord);

    ui->red_Button->setChecked(true);
}



MainWindow::~MainWindow()
{
    delete ui;
}

