#ifndef SCENE_H
#define SCENE_H

#include <QImage>
#include <vector>
#include "objects.h"


class Scene
{

protected:

    int width, height;      // render size (буфер)
    int vw = 0, vh = 0;      // viewport size (окно, геометрия)

    Projector proj;

    std :: vector<Object *> objects;
    std :: vector<LightSource> lights;

public:
    Scene(double w = 100, double h = 100, double l0 = 200);
    void setViewport(int w, int h);   // влияет на proj/камеру
    void resize(int w, int h);        //  только render size
    QImage render();

    Color compLight(Vec3f pt, size_t i, int planeIndex);
    Color TraceR(Ray r, int rec_d);

    void clear();
    void add_object(Object * o);
    void add_light(const LightSource& s);
    void tick();
};

#endif // SCENE_H
