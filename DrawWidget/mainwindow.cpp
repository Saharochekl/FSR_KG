#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

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

void MainWindow::getMouseCoord(QPointF point)
{
    double x_ = point.x();
    double y_ = point.y();
    QString textOutPut = "x_: " + QString::number(x_) + " y_: " +
            QString::number(y_);

    ui->textBrowser->setText(textOutPut);
}

void MainWindow::on_red_Button_clicked()
{
    ui->green_Button->setChecked(false);
    ui->widget->changedColor(Qt::red);
}

void MainWindow::on_green_Button_clicked()
{
    ui->red_Button->setChecked(false);
    ui->widget->changedColor(Qt::green);
}

void MainWindow::on_pushButton_deletePoints_clicked()
{
    ui->widget->clearVector();
    ui->textBrowser->clear();
}

void MainWindow::on_task1_clicked()
{
    ui->widget->setType(task1);
}

void MainWindow::on_task3_clicked()
{
    ui->widget->setType(task3);
}

void MainWindow::on_getResult_clicked()
{
    ui->widget->getResultOfTasks();
}

void MainWindow::on_task2_clicked()
{
    ui->widget->setType(task2);
}

void MainWindow::on_task4_clicked()
{
    ui->widget->setType(task4);
}

void MainWindow::on_task5_clicked()
{
   ui->widget->setType(task5);
}

void MainWindow::on_task6_clicked()
{
    ui->widget->setType(task6);
}
