#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "customdrawwidget.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
private slots:
    void getMouseCoord(QPointF point);

    void on_red_Button_clicked();
    void on_green_Button_clicked();
    void on_pushButton_deletePoints_clicked();
    void on_task1_clicked();
    void on_task3_clicked();
    void on_getResult_clicked();
    void on_task2_clicked();
    void on_task4_clicked();
    void on_task5_clicked();
    void on_task6_clicked();
    void checkIntersection();
    void updateIntersection();
};

#endif // MAINWINDOW_H
