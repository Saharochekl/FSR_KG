#ifndef SCENE_H
#define SCENE_H

#include <QImage>
#include <vector>
#include "objects.h"


class Scene
{

protected:

    int width;
    int height;
    int vw = 0, vh = 0;      // viewport size (окно, геометрия)

    Projector proj;

    std :: vector<Object *> objects;
    std :: vector<LightSource> lights;

public:
    Scene(double w = 100, double h = 100, double l0 = 200);
    QImage render();

    Color compLight(const Vec3f pt,const  size_t i);
    Color TraceR(Ray r, int rec_d);

    void clear();
    void add_object(Object * o);
    void add_light(const LightSource& s);
    void resize(int w, int h);
    void tick();

    void setViewport(int w, int h);    // влияет на projector/FOV
    void setRenderSize(int w, int h);  // влияет только на размер QImage/циклы
};

#endif // SCENE_H
