#include "qraytracingwidget.h"
#include <QPainter>
#include <QImage>
#include <iostream>
#include <Qtimer>
#include <math.h>

QRayTracingWidget::QRayTracingWidget(QWidget *parent)
    : QWidget{parent}, cur_sc(width(), height(), width() + height())
{
    Vec3f Orbit_center(-50, 200, 250);
    double orbit_radius = 300;


    Sphere* s1 = new Sphere(30, Vec3f(-50, 200, 550), Color(12, 100, 15), 70, 0.3);
    Sphere* s2 = new Sphere(30, Vec3f(-50 + orbit_radius*cos(2*M_PI/3), 200, 250 + orbit_radius*sin(2*M_PI/3)), Color(100, 50, 107), 70, 0.3);
    Sphere* s3 = new Sphere(30, Vec3f(-50 + orbit_radius*cos(4*M_PI/3), 200, 250 + orbit_radius*sin(4*M_PI/3)), Color(255, 10, 10), 70, 0.3);


//    cur_sc.add_object(new Sphere(30, Vec3f(-50, 250, 550), Color(12, 100, 15), 70, 0.4));
//    cur_sc.add_object(new Sphere(30, Vec3f(-50 + orbit_radius*cos(Angle), 250, 250 - orbit_radius*sin(Angle)), Color(100, 50, 107), 70, 0));
//    cur_sc.add_object(new Sphere(30, Vec3f(-50 - orbit_radius*cos(Angle), 250, 250 - orbit_radius*sin(Angle)), Color(255, 10, 10), 70, 0.4));


    s1->setOrbit(Orbit_center, orbit_radius, 0.0, 0.02);
    s2->setOrbit(Orbit_center, orbit_radius, 2*M_PI/3, 0.02);
    s3->setOrbit(Orbit_center, orbit_radius, 4*M_PI/3, 0.02);

    cur_sc.add_object(s1);
    cur_sc.add_object(s2);
    cur_sc.add_object(s3);
    // Напоминалка про vec3f(лево-право, выше-ниже, ближе-дальше)

    Star* star = new Star(Vec3f(-50, 100, 250), 200, Color(153, 204, 255), 70, 0.5);
    // Поворот вокруг оси X на 90 градусов
    double angleX = M_PI_2; // 90 градусов в радианах
    double angleY = 0.0;
    double angleZ = 0.0;
    star->rotating(angleX, angleY, angleZ);
    // Обновляем грани после трансформации
    star->updatePlanes();

    // Добавляем звезду в сцену
    cur_sc.add_object(star);

    Peaks4* peaks = new Peaks4(Vec3f(-50, -50, 0), 3, Color(75, 0, 130), -1, -1);

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
                                  Color(150, 255, 118), -1, -1));
    // плоскость(Зеркало)
    cur_sc.add_object(new Plane4v(Vec3f(-1100, 600, 6000),
                                  Vec3f(1100,  600, 6000),
                                  Vec3f(1100,  -600, 6000),
                                  Vec3f(-1100, -600, 6000),
                                  Color(240, 240, 240), 0, 1));

    cur_sc.add_light(LightSource(AMBIENT, Vec3f(0, 700, 0), 0.3));
    cur_sc.add_light(LightSource(POINT, Vec3f(700, 250, -500), 0.4));
    cur_sc.add_light(LightSource(POINT, Vec3f(-700, 250, -500), 0.4));

//    QTimer* timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, [this]() {
//        // Каждое срабатывание таймера будет обновлять сцену
//        cur_sc.tick();   // вызываем tick, чтобы объекты могли обновить своё состояние
//        update();        // перерисовать виджет
//    });
//    timer->start(3000); // обновление каждые 5 с (чтобы ретрейсинг успел произойти)

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



}
