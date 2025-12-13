#include "scene.h"

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
    QImage ret(width, height, QImage::Format_RGB32); // Создаем пустую картинку

    Color amb(150, 150, 150); //ПодсветОчка
    Vec3f org(-100, 300, -proj.get_l0());

    const double sx = double(vw) / double(width);
    const double sy = double(vh) / double(height);

    for (int h = 0; h < height; ++h) {
        auto* row = reinterpret_cast<uint32_t*>(ret.scanLine(h));
        for (int w = 0; w < width; ++w) {
            const double X = ((w + 0.5) * sx) - vw * 0.5;
            const double Y = vh * 0.5 - ((h + 0.5) * sy);

            Ray r(org, Vec3f(X, Y, 0) - org);
            Color c = TraceR(r, 2);

            row[w] = qRgb(c.R, c.G, c.B);
        }
    }
    return ret;
}

Color Scene::compLight(Vec3f pt, const size_t i){

    Color cur_col = Color(0,0,0);

    for(size_t l(0); l < lights.size(); l++)  { // цикл по всем источникам света
        LightSource& ls(lights[l]); // текущий источник света
        if (ls.type == AMBIENT)
        {
            cur_col = cur_col + ls.intensity * objects[i]->color;
        }
        else if(ls.type == POINT)
        {
            Vec3f ldir = ( ls.position - pt) * (1 / (pt - ls.position).length()); // Направляем вектор к источнику

            double cos = (ldir * objects[i]->get_normal(pt));

            if(cos >= 0)
            {
                cur_col = cur_col + ls.intensity * cos * objects[i]->color; // добавляем освещение
            }

            if(objects[i]->s != -1) // если оно должно блестеть
            {

                Vec3f R = (2 * objects[i]->get_normal(pt) * (ldir * objects[i]->get_normal(pt)) - ldir);
                double r_dot_v = R.norm() * pt.norm(); // Отражение

                if (r_dot_v < 0)
                {
                    cur_col = cur_col + ls.intensity * pow(r_dot_v, objects[i]->s) * objects[i]->color;
                }
            }
        }
    }
    return cur_col;
}

Color Scene::TraceR(Ray r, int rec_d){ // Сама трассировка луча

    double max_t = std::numeric_limits<double>::max();
    Color cur_col(0, 0, 0);

    size_t ind = -1;
    Vec3f pt;
    Vec3f pt2;
    double cur_t = -1;

    for(size_t i = 0; i < objects.size(); i++) // цикл по всем объектам сцены
    {
        cur_t = objects[i]->is_intersect(r); // Проверяем наличие пересечений
        pt = r(cur_t);
        if(cur_t > 0.0001)
        {
            if(cur_t < max_t)
            {
                pt2 = pt;
                cur_col = compLight(pt, i); // вычисляем цвет с учетом источников света
                ind = i;
                max_t = cur_t;
            }
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
            Vec3f N = objects[ind]->get_normal(pt2); // Нормаль к объекту в точке пересечения
            Vec3f Rq = (2 * N * ((-1 * r.dir) * N) + r.dir).norm(); // Направление отраженного луча

            Color reflected_c = TraceR(Ray(pt2, Rq), rec_d - 1); // Рекурсивная трассировка

            return cur_col * (1) + reflected_c * rr; // Возвращаем сумму цветов
        }
        else {
            return cur_col;
        }
    }
}
