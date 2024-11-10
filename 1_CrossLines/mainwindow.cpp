#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "mathfunctions.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->widget, &CustomDrawWidget::dataChanged,this,&MainWindow::getMouseCoord);
    //connect(ui->widget, &CustomDrawWidget::dataChanged, this, &MainWindow::addPoint);

    ui->red_Button->setChecked(true);
}

//void MainWindow::addPoint(QPointF point) {
//    // Добавляем точку в контейнер points
//    points.append(point);

//    // Отображаем текущие точки в TextBrowser для отладки
//    QString pointInfo = QString("Добавлена точка: (%1, %2)").arg(point.x()).arg(point.y());
//                            ui->textBrowser->append(pointInfo);
//}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_getResult_clicked()
{
    ui->widget->getResultOfTasks();
    switch (currentTaskType) {
    case Task1: {
        if (points.size() < 4) {
            ui->textBrowser->setText("Отрезки не заданы полностьюррррр.");
            return;
        }

        // Объявляем переменную для хранения точки пересечения
        QPointF intersection;
        QString tmp = QString("Size of points Vec(%1).")
                             .arg(points.size());
        ui->textBrowser->append(tmp);
        // Проверяем пересечение и вычисляем точку пересечения
        bool intersect = doIntersect(points[0], points[1], points[2], points[3], intersection);
        if (intersect) {
            QString result = QString("Отрезки пересекаются в точке (%1, %2).")
                                 .arg(intersection.x())
                                 .arg(intersection.y());
            ui->textBrowser->append(result);
        } else {
            ui->textBrowser->append("Отрезки не пересекаются.");
        }
        break;
    }
    case Task2: {
        if (points.size() < 3) {
            ui->textBrowser->setText("Точки не заданы полностью для предиката поворота.");
            return;
        }
        QString tmp = QString("Size of points Vec(%1).")
                          .arg(points.size());
        ui->textBrowser->append(tmp);
        int o = orientation(points[0], points[1], points[2]);
        QString result;
        if (o == 0) {
            result = "Straight.";
        } else if (o == 1) {
            result = "Left.";
        } else {
            result = "Right.";
        }
        ui->textBrowser->append(result);
        break;
    }
    case Task3: {
        if (points.size() < 3) {
            ui->textBrowser->setText("Для выпуклой оболочки нужно минимум 3 точки.");
            return;
        }
        // Вызов функции построения выпуклой оболочки и отображение результата
        ui->textBrowser->setText("Выпуклая оболочка построена.");
        break;
    }
    case Task4:
        ui->textBrowser->setText("Задача триангуляции Делоне будет реализована позже.");
        break;
    case Task5:
        ui->textBrowser->setText("Задача для работы с произвольными многоугольниками будет реализована позже.");
        break;
    case Task6:
        ui->textBrowser->setText("Задача локализации точки будет реализована позже.");
        break;
    }
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
    // Очищаем вектор с точками
    points.clear();

    // Обновляем интерфейс для отображения изменений
    ui->textBrowser->append("Все точки удалены.");
    ui->widget->clearVector();
    ui->textBrowser->clear();
}

void MainWindow::on_task1_clicked()
{
    ui->widget->setType(task1);
    currentTaskType = Task1;
    points.clear();
    ui->textBrowser->setText("Выбрано: Пересечение двух отрезков");

}

void MainWindow::on_task3_clicked()
{
    ui->widget->setType(task3);
    currentTaskType = Task3;
    points.clear(); // Выпуклая оболочка требует произвольное количество точек
    ui->textBrowser->setText("Выбрано: Построение выпуклой оболочки");
}



void MainWindow::on_task2_clicked()
{
    ui->widget->setType(task2);
    currentTaskType = Task2;
    points.clear();
    //points.resize(3); // Предикат поворота требует 3 точки
    ui->textBrowser->setText("Выбрано: Предикат поворота");
}

void MainWindow::on_task4_clicked()
{
    ui->widget->setType(task4);
    currentTaskType = Task4;
    points.clear();
    ui->textBrowser->setText("Выбрано: Триангуляция Делоне");
}

void MainWindow::on_task5_clicked()
{
   ui->widget->setType(task5);
    currentTaskType = Task5;
    points.clear();
    ui->textBrowser->setText("Выбрано: Пересечение произвольных многоугольников");
}

void MainWindow::on_task6_clicked()
{
    ui->widget->setType(task6);
   currentTaskType = Task6;
   points.clear();
   ui->textBrowser->setText("Выбрано: Локализация точки относительно многоугольника");
}
