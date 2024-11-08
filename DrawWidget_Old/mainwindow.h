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
    Ui::MainWindow *ui;
private slots:
    void getMouseCoord(QPoint point);

    void on_red_Button_clicked();
    void on_green_Button_clicked();
};

#endif // MAINWINDOW_H
