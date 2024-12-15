#include "qraytracingwidget.h"
#include <QPainter>
#include <QImage>
#include <iostream>


QRayTracingWidget::QRayTracingWidget(QWidget *parent)
    : QWidget{parent}, cur_sc(width(), height(), width() + height())
{
    cur_sc.add_object(new Sphere(130, Vec3f(300, 50, 200), Color(12, 100, 15), 20, 0.4));
    cur_sc.add_object(new Sphere(90, Vec3f(250, 170, 350), Color(100, 50, 107), 70, 0));
    cur_sc.add_object(new Sphere(30, Vec3f(100, 50, -150), Color(255, 0, 0), 20, 0.3));

    // Напоминалка про vec3f(лево-право, выше-ниже, ближе-дальше)

    Star* star = new Star(Vec3f(-50, 100, 250), 200, Color(255, 255, 0));
    // Поворот вокруг оси X на 90 градусов (M_PI_2 — это π/2, если не определено, можно использовать 1.5708)
    double angleX = M_PI_2; // 90 градусов в радианах
    double angleY = 0.0;
    double angleZ = 0.0;
    star->rotating(angleX, angleY, angleZ);
    // Обновляем грани после трансформации
    star->updatePlanes();

    // Добавляем звезду в сцену
    cur_sc.add_object(star);

    Peaks4* peaks = new Peaks4(Vec3f(-50, -50, 0), 3, Color(0, 0, 255));

     angleX = 0.0; // 90 градусов в радианах
     angleY = M_PI/4;
     angleZ = 0.0;
    peaks->rotating(angleX, angleY, angleZ);
    // Обновляем грани после трансформации
    peaks->updatePlanes();

    cur_sc.add_object(peaks);
    // плоскость
    cur_sc.add_object(new Plane4v(Vec3f(-1100, -600, 100),
                                  Vec3f(1100,  -600, 100),
                                  Vec3f(1100,  -600, 8000),
                                  Vec3f(-1100, -600, 8000),
                                  Color(150, 72, 118), -1, -1));
    // плоскость
    cur_sc.add_object(new Plane4v(Vec3f(-1100, 600, 6000),
                                  Vec3f(1100,  600, 6000),
                                  Vec3f(1100,  -600, 6000),
                                  Vec3f(-1100, -600, 6000),
                                  Color(240, 240, 240), 0, 1));

    cur_sc.add_light(LightSource(AMBIENT, Vec3f(), 0.3));
    cur_sc.add_light(LightSource(POINT, Vec3f(700, 500, 0), 0.8));
    cur_sc.add_light(LightSource(POINT, Vec3f(-700, 250, -1000), 0.1));

//    QTimer* timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, [this]() {
//        // Каждое срабатывание таймера будет обновлять сцену
//        cur_sc.tick();   // вызываем tick, чтобы объекты могли обновить своё состояние
//        update();        // перерисовать виджет
//    });
//    timer->start(5000); // обновление каждые 5 с (чтобы ретрейсинг успел произойти)
}

const int chess_cell_size = 30;

void QRayTracingWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    int wW = width();
    int hW = height();




    cur_sc.resize(wW, hW);
    QImage img = cur_sc.render();
    p.drawImage(0, 0, img);

    QPen cpen(Qt::red);
    cpen.setWidth(3);
    QPen oldpen = p.pen();
    p.setPen(cpen);
    p.drawRect(0, 0, wW - 1, hW - 1) ;
    QString qs = QString :: asprintf("(%d,%d)", wW, hW);
    p.drawText(p.window().width() / 2, p.window().height() - 20, qs);
    p.setPen(oldpen);
}

void QRayTracingWidget::MakeFilm()
{


    int num_frames = 100;
    for(int nf = 0; nf < num_frames; nf++)
    {
        cur_sc.tick();
        QImage img = cur_sc.render();
        QString fname_img = QString::asprintf("img_%03d.png", nf);
        img.save(fname_img);
        std :: cerr << "Pict " << fname_img.toStdString() << " was output" << std :: endl;
    }
    system("mencoder \"mf:\/\/\*.png\" -vf scale=1400:-11 -oac copy -ovc lavc -lavcopts vcodec=msmpeg4:vbitrate=2000 -ffourcc MP43 -o \"output2.avi\"");
}
