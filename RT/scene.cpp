#include "scene.h"
#include <thread>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <QVector>
#include <QtConcurrent/QtConcurrent>




Scene::Scene(double w, double h, double l0)
    : width(w), height(h), proj(l0) // Инициализация параметров сцены
{

}

void Scene::tick()
{
    for(size_t q(0); q < objects.size(); q++)
        objects[q]->tick(); // Обновляем состояние всех объектов
}

void Scene:: clear(){
     objects.clear();
}

void Scene:: add_object(Object * o){
     objects.push_back(o);
}

void Scene:: add_light(const LightSource& s) {
     lights.push_back(s);
}

void Scene::setViewport(int w, int h)
{
     vw = w;
     vh = h;
     proj = Projector(vw + vh); // или другой твой выбор, но точно НЕ от render-size
}

void Scene:: resize(int w, int h)
{
     width = w;
     height = h;
}


QImage Scene::render()
{
     QImage ret(width, height, QImage::Format_RGB32);
     if (width <= 0 || height <= 0 || vw <= 0 || vh <= 0) return ret;

     ret.detach();
     auto* base = reinterpret_cast<QRgb*>(ret.bits());
     const int stride = ret.bytesPerLine() / int(sizeof(QRgb));

     const int H = height, W = width;
     const double sx = double(vw) / double(W);
     const double sy = double(vh) / double(H);
     const Vec3f org(-100, 300, -proj.get_l0());

     auto clamp8 = [](int v){ return v < 0 ? 0 : (v > 255 ? 255 : v); };

     const int block = 16;
     QVector<int> starts;
     starts.reserve((H + block - 1) / block);
     for (int y = 0; y < H; y += block) starts.push_back(y);

     QtConcurrent::blockingMap(starts, [&](int y0){
         const int y1 = std::min(H, y0 + block);
         for (int h = y0; h < y1; ++h) {
             QRgb* row = base + h * stride;     // <-- без scanLine
             const double Y = vh * 0.5 - ((h + 0.5) * sy);
             for (int w = 0; w < W; ++w) {
                 const double X = ((w + 0.5) * sx) - vw * 0.5;
                 Ray r(org, Vec3f(X, Y, 0) - org);
                 Color c = TraceR(r, 2);
                 row[w] = qRgb(clamp8(c.R), clamp8(c.G), clamp8(c.B));
             }
         }
     });

     return ret;
}



Color Scene::compLight(Vec3f pt, const size_t i, int planeIndex)
{
     Color cur_col(0,0,0);
     Vec3f N = objects[i]->get_normal(pt, planeIndex).Normilize();

     for (size_t l = 0; l < lights.size(); ++l) {
        LightSource& ls(lights[l]);
        if (ls.type == AMBIENT) {
            cur_col = cur_col + ls.intensity * objects[i]->color;
        } else if (ls.type == POINT) {
            Vec3f ldir = (ls.position - pt).Normilize();
            double cos = (ldir * N);

            if (cos >= 0) {
                cur_col = cur_col + ls.intensity * cos * objects[i]->color;
            }

            if (objects[i]->s != -1) {
                Vec3f R = (2 * N * (ldir * N) - ldir).Normilize();
                double r_dot_v = R * pt.norm(); // это у тебя сомнительно, но оставляю как было
                if (r_dot_v < 0) {
                    cur_col = cur_col + ls.intensity * pow(r_dot_v, objects[i]->s) * objects[i]->color;
                }
            }
        }
     }
     return cur_col;
}

static inline bool rayHitsSphere(const Ray& r, const Vec3f& c, double R)
{
    Vec3f oc = r.beg - c;

    const double a  = r.dir * r.dir;
    const double b  = 2.0 * (oc * r.dir);
    const double cc = (oc * oc) - R*R;

    const double disc = b*b - 4.0*a*cc;
    if (disc < 0.0) return false;

    const double s = sqrt(disc);
    const double t1 = (-b - s) / (2.0*a);
    const double t2 = (-b + s) / (2.0*a);

    return (t1 > 1e-4) || (t2 > 1e-4);
}


Color Scene::TraceR(Ray r, int rec_d){ // Сама трассировка луча

    double max_t = std::numeric_limits<double>::max();
    Color cur_col(0, 0, 0);

    size_t ind = -1;
    Vec3f pt;
    Vec3f pt2;
    double cur_t = -1;
    int bestPlane = -1;

    for(size_t i = 0; i < objects.size(); ++i) // цикл по всем объектам сцены
    {
        const double R = objects[i]->boundRadius();
        if (R > 0.0) {
            if (!rayHitsSphere(r, objects[i]->ctr(), R))
                continue; // луч мимо bounding sphere -> тяжелый тест не нужен
        }
        auto hit = objects[i]->intersect(r);
        cur_t = hit.t;
        pt = r(cur_t);

        if (cur_t > 0.0001 && cur_t < max_t) {
            pt2 = pt;
            ind = i;
            max_t = cur_t;
            bestPlane = hit.planeIndex;
            cur_col = compLight(pt, i, bestPlane); // см. шаг D
        }
    }

    if(ind == -1)
    {
        return cur_col; // Если не было пересечений, возвращаем текущий цвет
    }
    else {
        double rr = objects[ind]->r; // коэффициент отражения объекта
        if(rec_d <= 0 || rr == -1)
        { // если достигнута максимальная глубина трассировки или объект не отражает свет
            return cur_col; // возвращаем текущий цвет
        }


        if(max_t > 0.0001 && max_t != std::numeric_limits<double>::max()) // если было пересечение
        {
//            Vec3f N = objects[ind]->get_normal(pt2, bestPlane);  // Нормаль к объекту в точке пересечения
//            Vec3f Rq = (2 * N * ((-1 * r.dir) * N) + r.dir).norm(); // Направление отраженного луча

//            Color reflected_c = TraceR(Ray(pt2, Rq), rec_d - 1); // Рекурсивная трассировка

//            return cur_col * (1) + reflected_c * rr; // Возвращаем сумму цветов
            Vec3f hit = r(max_t); // точка пересечения

            Vec3f N = objects[ind]->get_normal(pt2, bestPlane).Normilize();
            if ((r.dir * N) > 0) {
                N = (-1) * N;
            }
                // смещение, чтобы не ловить самопересечение
            const double eps = 1e-4;
            Vec3f origin = hit + eps * N;

            Vec3f Rq = (r.dir - 2.0 * N * (r.dir * N)).Normilize();

            Color reflected_c = TraceR(Ray(origin, Rq), rec_d - 1);

            return cur_col * (1.0 - rr) + reflected_c * rr;

        }
        else {
            return cur_col;
        }
    }
}
