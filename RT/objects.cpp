#include "objects.h"


void Star::updatePlanes() {
    planes.clear();
//    planes = {
//              Plane3v(vertices[6], vertices[10], vertices[0], color),
//              Plane3v(vertices[6] , vertices[1], vertices[10], color),
//              Plane3v(vertices[3] , vertices[10] , vertices[5] , color),
//              Plane3v(vertices[5]  , vertices[10] , vertices[1] , color),
//              Plane3v(vertices[9]  , vertices[4] , vertices[2] , color),
//              Plane3v(vertices[2]  , vertices[7] , vertices[9] , color),
//              Plane3v(vertices[7]  , vertices[3] , vertices[9] , color),
//              Plane3v(vertices[3]  , vertices[5] , vertices[9] , color),
//              Plane3v(vertices[5]  , vertices[1] , vertices[9] , color),
//              Plane3v(vertices[9]  , vertices[6] , vertices[0] , color),
//              Plane3v(vertices[6]  , vertices[9] , vertices[1] , color),
//              Plane3v(vertices[4] , vertices[9] , vertices[0] , color),
//              Plane3v(vertices[7] , vertices[10] , vertices[3] , color),
//              Plane3v(vertices[2] , vertices[10] , vertices[7] , color),
//              Plane3v(vertices[4] , vertices[10] , vertices[2] , color),
//              Plane3v(vertices[0] , vertices[10] , vertices[4] , color)
//    };

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
