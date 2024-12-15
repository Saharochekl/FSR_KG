#include "objects.h"







//
//Конструкторы Star
//

Star::Star() : Object(Color()), lastPlaneIndex(-1), scale(1.0), rotationAngle(0.0) {
    // Тут можно оставить пусто или инициализировать поля по умолчанию, мб потом чёт поменяю
}

Star::Star(const Vec3f& c, double scaleFactor, Color col)
    : Object(col), lastPlaneIndex(-1), scale(scaleFactor), center(c)
{
    s = -1;
    r = -1;
    scale = scaleFactor;
    center = c;

    // Определяем вершины
    vertices = {
        Vec3f(-0.198617, 0.000000, 0.198617),
        Vec3f(0.200833, 0.000000, 0.198195),
        Vec3f(-0.197404, 0.000000, -0.202679),
        Vec3f(0.198195, 0.000000, -0.200833),
        Vec3f(-0.599098, 0.000000, 0.000000),
        Vec3f(0.596460, 0.000000, 0.000000),
        Vec3f(0.000000, 0.000000, 0.603001),
        Vec3f(0.000000, 0.000000, -0.599098),
        Vec3f(0.000000, 0.000000, 0.000000),
        Vec3f(0.000000, 0.098429, 0.000000),
        Vec3f(0.000000, -0.101898, 0.000000)
    };

    // Определяем грани (треугольники)
    planes = {
        Plane3v(vertices[6]* scale   + center, vertices[10]* scale   + center, vertices[0]* scale + center, col),
        Plane3v(vertices[6]* scale   + center, vertices[1]* scale  + center, vertices[10]* scale  + center, col),
        Plane3v(vertices[3]* scale   + center, vertices[10]* scale  + center, vertices[5]* scale  + center, col),
        Plane3v(vertices[5]* scale   + center, vertices[10]* scale  + center, vertices[1]* scale   + center, col),
        Plane3v(vertices[9]* scale   + center, vertices[4]* scale   + center, vertices[2]* scale   + center, col),
        Plane3v(vertices[2]* scale   + center, vertices[7]* scale   + center, vertices[9]* scale   + center, col),
        Plane3v(vertices[7]* scale   + center, vertices[3]* scale   + center, vertices[9]* scale   + center, col),
        Plane3v(vertices[3]* scale   + center, vertices[5]* scale   + center, vertices[9]* scale   + center, col),
        Plane3v(vertices[5]* scale   + center, vertices[1]* scale   + center, vertices[9]* scale   + center, col),
        Plane3v(vertices[9]* scale   + center, vertices[6]* scale   + center, vertices[0]* scale   + center, col),
        Plane3v(vertices[6]* scale   + center, vertices[9]* scale   + center, vertices[1]* scale   + center, col),
        Plane3v(vertices[4]* scale   + center, vertices[9]* scale   + center, vertices[0]* scale   + center, col),
        Plane3v(vertices[7]* scale   + center, vertices[10]* scale   + center, vertices[3]* scale   + center, col),
        Plane3v(vertices[2]* scale   + center, vertices[10]* scale   + center, vertices[7]* scale   + center, col),
        Plane3v(vertices[4]* scale   + center, vertices[10]* scale   + center, vertices[2]* scale   + center, col),
        Plane3v(vertices[0]* scale   + center, vertices[10]* scale   + center, vertices[4]* scale   + center, col)
    };

}

//
//Функции Star
//

void Star::updatePlanes() {
    planes.clear();
    planes = {
        Plane3v(vertices[6]* scale   + center, vertices[10]* scale   + center, vertices[0]* scale + center, color),
        Plane3v(vertices[6]* scale   + center, vertices[1]* scale  + center, vertices[10]* scale  + center, color),
        Plane3v(vertices[3]* scale   + center, vertices[10]* scale  + center, vertices[5]* scale  + center, color),
        Plane3v(vertices[5]* scale   + center, vertices[10]* scale  + center, vertices[1]* scale   + center, color),
        Plane3v(vertices[9]* scale   + center, vertices[4]* scale   + center, vertices[2]* scale   + center, color),
        Plane3v(vertices[2]* scale   + center, vertices[7]* scale   + center, vertices[9]* scale   + center, color),
        Plane3v(vertices[7]* scale   + center, vertices[3]* scale   + center, vertices[9]* scale   + center, color),
        Plane3v(vertices[3]* scale   + center, vertices[5]* scale   + center, vertices[9]* scale   + center, color),
        Plane3v(vertices[5]* scale   + center, vertices[1]* scale   + center, vertices[9]* scale   + center, color),
        Plane3v(vertices[9]* scale   + center, vertices[6]* scale   + center, vertices[0]* scale   + center, color),
        Plane3v(vertices[6]* scale   + center, vertices[9]* scale   + center, vertices[1]* scale   + center, color),
        Plane3v(vertices[4]* scale   + center, vertices[9]* scale   + center, vertices[0]* scale   + center, color),
        Plane3v(vertices[7]* scale   + center, vertices[10]* scale   + center, vertices[3]* scale   + center, color),
        Plane3v(vertices[2]* scale   + center, vertices[10]* scale   + center, vertices[7]* scale   + center, color),
        Plane3v(vertices[4]* scale   + center, vertices[10]* scale   + center, vertices[2]* scale   + center, color),
        Plane3v(vertices[0]* scale   + center, vertices[10]* scale   + center, vertices[4]* scale   + center, color)
    };
}


// Перемещение центра звезды
void Star::moving(const Vec3f& delta) {
    for (auto& vertex : vertices) {
        vertex = vertex + delta;
    }
    center = center + delta;
}

// Масштабирование звезды
void Star::scaling(double factor) {
    for (auto& vertex : vertices) {
        vertex = center + (vertex - center) * factor;
    }
}
