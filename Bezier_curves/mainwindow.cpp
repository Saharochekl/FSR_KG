#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "customdrawwidget.h"

#include "bezierutils.h"
#include "longdouble.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->widget, &CustomDrawWidget::dataChanged,this,&MainWindow::getMouseCoord);
    connect(ui->Linear_curve, &QCheckBox::stateChanged, this, &MainWindow::onCheckboxStateChanged);
    connect(ui->Qadratic_curve, &QCheckBox::stateChanged, this, &MainWindow::onCheckboxStateChanged);
    connect(ui->Cubiccurve, &QCheckBox::stateChanged, this, &MainWindow::onCheckboxStateChanged);
    connect(ui->Manual_curve, &QCheckBox::stateChanged, this, &MainWindow::onCheckboxStateChanged);

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Draw_curve_clicked()
{
    if(points.size() < 2){
        ui->textBrowser->append("Для кривой нужно хотя бы 2 точки! ");
        return;
    }
    //int degree = 4; // Степень по умолчанию
    if (isManualChecked) {
        bool ok;
        int inputDegree = ui->Pow_of_curve->toPlainText().toInt(&ok);
        if (ok && inputDegree > 1) {
            ui->widget->setDegree(inputDegree);
            ui->textBrowser->append("Степень установлена: " + QString::number(inputDegree));
        } else {
            ui->textBrowser->append("Некорректная степень, используется значение по умолчанию (4)");
        }
        ui->textBrowser->append("Отображена кривая с заданной степенью " + QString::number(inputDegree));
    }

    //ui->textBrowser->append("Выбрана степень: " + QString::number(degree));

    // Пока что просто вызываем repaint для обновления
    ui->widget->repaint();


    if(isLinearChecked){
        ui->textBrowser->append("Отображена линейная кривая");
    }
    if(isQuadraticChecked){
        ui->textBrowser->append("Отображена квадратичная кривая");

    }
    if(isCubicChecked){
        ui->textBrowser->append("Отображена кубическая кривая");

    }
}

void MainWindow::on_Deletepoints_clicked()
{
    points.clear();
    ui->widget->clearVector();

    ui->textBrowser->append("Поле очищено");
    ui->widget->repaint();
}

void MainWindow::getMouseCoord(QPointF point)
{
    points.append(point);
    double x_ = point.x();
    double y_ = point.y();
    QString textOutPut = "x_: " + QString::number(x_) + " y_: " +
                         QString::number(y_);
    ui->textBrowser->append(textOutPut);
}

void MainWindow::onCheckboxStateChanged(int state) {
    QCheckBox* senderCheckBox = qobject_cast<QCheckBox*>(sender());
    if (!senderCheckBox) return;

    if (senderCheckBox == ui->Linear_curve) {
        isLinearChecked = (state == Qt::Checked);
        ui->widget->isLinearChecked = isLinearChecked; // Обновляем состояние в CustomDrawWidget
        ui->textBrowser->append("Выбрана линейная кривая");
    }

    if (senderCheckBox == ui->Qadratic_curve) {
        isQuadraticChecked = (state == Qt::Checked);
        ui->widget->isQuadraticChecked = isQuadraticChecked; // Обновляем правильное состояние
        ui->textBrowser->append("Выбрана квадратичная кривая");
    }

    if (senderCheckBox == ui->Cubiccurve) {
        isCubicChecked = (state == Qt::Checked);
        ui->widget->isCubicChecked = isCubicChecked; // Обновляем правильное состояние
        ui->textBrowser->append("Выбрана кубическая кривая");
    }

    if (senderCheckBox == ui->Manual_curve) {
        isManualChecked = (state == Qt::Checked);
        ui->widget->isManualChecked = isManualChecked; // Обновляем правильное состояние
        ui->textBrowser->append("Выбрана кривая заданной степени");
    }

    ui->widget->update(); // Перерисовываем виджет
}


