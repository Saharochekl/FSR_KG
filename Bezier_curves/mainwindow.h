#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "customdrawwidget.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    bool isLinearChecked = false;
    bool isQuadraticChecked = false;
    bool isCubicChecked = false;
    bool isManualChecked = false;

    QVector<QPointF> points; // Контейнер для хранения точек
    Ui::MainWindow *ui;

private slots:
    void getMouseCoord(QPointF point);
    void on_Draw_curve_clicked();
    void on_Deletepoints_clicked();
    void onCheckboxStateChanged(int state);
   // void on_getResult_clicked();


};

#endif // MAINWINDOW_H
