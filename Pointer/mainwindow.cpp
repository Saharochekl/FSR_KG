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

void MainWindow::getMouseCoord(QPoint point)
{
    std::cout << "Get signal" << std::endl;
    int x_ = point.x();
    int y_ = point.y();
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
