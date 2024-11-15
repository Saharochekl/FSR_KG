#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "mathfunctions.h"
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
        QString tmp = QString("Size of points Vec(%1).")
                          .arg(points.size());
        ui->textBrowser->append(tmp);
        if (points.size() < 3) {
            ui->textBrowser->setText("Точки не заданы полностью для предиката поворота.");
            return;
        }
        int o = orientation(points[0], points[1], points[2]);
        QString result;
        if (o == 0) {
            result = "Straight.";
        } else if (o == 1) {
            result = "Right.";
        } else {
            result = "Left.";
        }
        ui->textBrowser->append(result);
        break;
    }
    case Task3: {
        if (points.size() < 3) {
            ui->textBrowser->append("Недостаточно точек для построения выпуклой оболочки.");
            return;
        }

        // Вызываем алгоритм Джарвиса для построения выпуклой оболочки
        QVector<QPointF> hull = jarvisConvexHull(points);

        // Сохраняем точки выпуклой оболочки в отдельный вектор
        ui->widget->hullPoints = hull;

        // Обновляем виджет для отображения новой оболочки
        ui->widget->update();

        // Отображаем информацию о построенной оболочке в textBrowser
        ui->textBrowser->append("Выпуклая оболочка построена. Количество точек: " + QString::number(hull.size()));
        break;
    }
    case Task4:
    {
        if (points.size() < 3) {
            ui->textBrowser->append("Недостаточно точек для выполнения триангуляции.");
            return;
        }
        QStringList logMessages;  // Логи рёбер

        QVector<Edge> triangulationEdges = getTriangulation(points, logMessages);

        // Выводим логированные сообщения
        ui->textBrowser->append("Логи рёбер:");
        for (const QString &msg : logMessages) {
            ui->textBrowser->append(msg);
        }
        // Устанавливаем рёбра триангуляции для отрисовки
        ui->widget->triangulationEdges = triangulationEdges;
        ui->widget->update();

        // Отображаем информацию о триангуляции
        ui->textBrowser->append("Триангуляция выполнена. Количество рёбер: " + QString::number(triangulationEdges.size()));
        break;
    }
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

void MainWindow::on_Add_point_manual_clicked()
{
    // Читаем значения x и y из текстовых полей
    QString xStr = ui->Xcoord->toPlainText();  // Используем toPlainText(), так как это QTextEdit
    QString yStr = ui->Ycoord->toPlainText();

    // Преобразуем строковые значения в LongDouble
    LongDouble x, y;

    try {
        x = LongDouble(xStr.toStdString());
        y = LongDouble(yStr.toStdString());
        ui->textBrowser->append("Добаление успешно, преобразование сделано");
    } catch (const std::exception& e) {
        ui->textBrowser->append("Ошибка: Неверный формат координат");
        return;
    }

    // Создаём точку и добавляем её в вектор точек
    QPointF newPoint(x.toDouble(), y.toDouble());  // Используем toDouble() для создания точки, которую можно отобразить на виджете
    points.append(newPoint);
    ui->widget->vecPoint.push_back(newPoint);

    // Обновляем виджет для отображения новой точки
    ui->widget->update();

    // Выводим сообщение об успешном добавлении точки
    ui->textBrowser->append(QString("Добавлена точка: (%1, %2)").arg(xStr).arg(yStr));
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
    points.clear();
    ui->textBrowser->setText("Выбрано: Построение выпуклой оболочки");
}



void MainWindow::on_task2_clicked()
{
    ui->widget->setType(task2);
    currentTaskType = Task2;
    points.clear();
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


void MainWindow::on_newPoly_clicked()
{
   if (currentTaskType == Task5)
   {
        ui->widget->isFirstPolygon = false;
        // Замыкаем первый многоугольник
        if (!ui->widget->Polygon1.isEmpty()) {
            ui->widget->Polygon1.append(ui->widget->Polygon1.first());
        }
        ui->widget->update();
        ui->textBrowser->append("Первый многоугольник завершён. Рисуем второй многоугольник.");
   }
}
