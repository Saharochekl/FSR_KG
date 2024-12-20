#include "objects.h"


//
// Конструкторы Vec3f
//

Vec3f:: Vec3f(){}

Vec3f:: Vec3f(double inX, double inY, double inZ): x(inX), y(inY), z(inZ){

}

//
// Функции Vec3f
//


//
// Конструкторы Color
//

Color:: Color() : R(0), G(0), B(0) { }
Color:: Color(unsigned short iR,
      unsigned short iG,
      unsigned short iB) : R(iR), G(iG), B(iB) { }


//
// Конструкторы Ray
//

Ray::Ray() { }
Ray:: Ray(const Vec3f& b, const Vec3f& d) : beg(b), dir(d) { }

//
// Конструкторы LightSourse
//

LightSource:: LightSource() { }
LightSource:: LightSource(int t, const Vec3f& v, const double& intens)
    : type(t), position(v), intensity(intens) { }


//
// Конструкторы Object
//

Object:: Object() { }
Object:: Object(Color c) : color(c) { }


//
// Конструкторы Sphere
//


Sphere:: Sphere() : orbitRadius(0), angle(0), angleStep(0) { }
Sphere:: Sphere(const double& rad, const Vec3f& v): radius(rad), center(v), dir(0, 1, 0), step(5), phase(0), amp(300), orbitRadius(0), angle(0), angleStep(0) { }
Sphere:: Sphere(const double& rad,
       const Vec3f& v,
       Color c)  : Object(c), radius(rad), center(v), dir(0, 1, 0), step(5), phase(0), amp(300), orbitRadius(0), angle(0), angleStep(0) {r=-1; s=-1;}


Sphere:: Sphere(const double& rad,
       const Vec3f& v,
       Color c,  double spect, double refl) : Object(c), radius(rad), center(v), dir(0,1,0), step(5), phase(0), amp(300), orbitRadius(0), angle(0), angleStep(0) {r = refl; s = spect;}


Sphere:: Sphere(const double& rad,
       const Vec3f& v,
       Color c,
       double in_step,
       Vec3f in_dir,
       double in_amp) :
    Object(c), radius(rad), center(v),
    dir(in_dir), step(in_step), phase(0), amp(in_amp), orbitRadius(0), angle(0), angleStep(0) { }
//
// Функции Sphere
//


Vec3f Sphere:: get_normal(const Vec3f& v) const
{
    return (v - center).Normilize();
}

Vec3f Sphere:: gep_pos() const
{
    return center + phase * dir;
}

double Sphere:: is_intersect(const Ray& r) const
{
    double A = r.dir.sq();
    Vec3f v = r.beg - gep_pos();
    double B = r.dir * v;
    double C = v.sq() - radius * radius;

    double D = B * B - A * C;

    if(D < 0)
    {
        return -1.0;
    }
    double sD = sqrt(D);
    double t1 = (-B - sD) / A;
    double t2 = (-B + sD) / A;

    return (t1 > 0) ? t1 : t2;
}

void Sphere:: tick()
{
    // Если orbitRadius > 0, значит сфера движется по орбите
    if(orbitRadius > 0)
    {
        // Фиксируем высоту
        double fixedY = center.y;

        angle += angleStep;
        // Движение по кругу в плоскости XZ:
        double newX = orbitCenter.x + orbitRadius * cos(angle);
        double newZ = orbitCenter.z + orbitRadius * sin(angle);

        center = Vec3f(newX, fixedY, newZ);
    }


}

void Sphere:: setOrbit(const Vec3f& orbitC, double radius, double initialAngle, double stepAngle)
{
    orbitCenter = orbitC;
    orbitRadius = radius;
    angle = initialAngle;
    angleStep = stepAngle;
}

//
// Конструкторы Plane3v
//

Plane3v:: Plane3v() : Object(Color()) { } // dummy object
Plane3v:: Plane3v(const Vec3f& A,
        const Vec3f& B,
        const Vec3f& C,
        Color col) : Object(col)
{
    vert[0] = A;
    vert[1] = B;
    vert[2] = C;
    s = -1;
    r = -1;
}
Plane3v:: Plane3v(const Vec3f& A,
        const Vec3f& B,
        const Vec3f& C,
        Color col, double spect, double refl) : Object(col)
{
    vert[0] = A;
    vert[1] = B;
    vert[2] = C;
    s = spect;
    r = refl;
}


//
// Функции Plane3v
//

double Plane3v:: is_intersect(const Ray& r) const
{
    return is_intersect_tr(r, vert[0], vert[1], vert[2]);

}

Vec3f Plane3v:: get_normal(const Vec3f& v) const
{
    return ((vert[2] - vert[0]) % (vert[1] - vert[0])).Normilize();
}

//
// Конструкторы Plane4v
//

Plane4v:: Plane4v() : Object(Color()) { }
Plane4v:: Plane4v(const Vec3f& A,
        const Vec3f& B,
        const Vec3f& C,
        const Vec3f& D,
        Color col) : Object(col)
{
    vert[0] = A;
    vert[1] = B;
    vert[2] = C;
    vert[3] = D;
    s = -1;
    r = -1;
}
Plane4v:: Plane4v(const Vec3f& A,
        const Vec3f& B,
        const Vec3f& C,
        const Vec3f& D,
        Color col, double spect, double refl) : Object(col)
{
    vert[0] = A;
    vert[1] = B;
    vert[2] = C;
    vert[3] = D;
    s = spect;
    r = refl;
}

//
// Функции Plane4v
//

double Plane4v:: is_intersect(const Ray& r) const
{
    double t1 = is_intersect_tr(r, vert[0], vert[1], vert[2]);
    if(t1 > 0)
        return t1;

    return is_intersect_tr(r, vert[2], vert[3], vert[0]);
}

Vec3f Plane4v:: get_normal(const Vec3f& v) const
{
    return ((vert[2] - vert[0]) % (vert[1] - vert[0])).Normilize();
}


//
//Конструкторы Star
//

Star::Star() : Object(Color()), lastPlaneIndex(-1), scale(1.0), rotationAngle(0.0) {
}

Star::Star(const Vec3f& c, double scaleFactor, Color col, double spect, double refl)
    : Object(col), lastPlaneIndex(-1), scale(scaleFactor), center(c)
{
    s = spect;
    r = refl;
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
    center = center + delta;
}

// Масштабирование звезды
void Star::scaling(double factor) {
    for (auto& vertex : vertices) {
        vertex = center + (vertex - center) * factor;
    }
}

// Вращение вокруг осей
void Star:: rotating(double angleX, double angleY, double angleZ) {
    for (auto& vertex : vertices) {
        // Приводим вершину к локальной системе координат звезды
        Vec3f localVertex = vertex;

        // Вращение вокруг оси X
        double sinX = sin(angleX);
        double cosX = cos(angleX);
        localVertex = Vec3f(
            localVertex.x,
            localVertex.y * cosX - localVertex.z * sinX,
            localVertex.y * sinX + localVertex.z * cosX
            );

        // Вращение вокруг оси Y
        double sinY = sin(angleY);
        double cosY = cos(angleY);
        localVertex = Vec3f(
            localVertex.x * cosY + localVertex.z * sinY,
            localVertex.y,
            -localVertex.x * sinY + localVertex.z * cosY
            );

        // Вращение вокруг оси Z
        double sinZ = sin(angleZ);
        double cosZ = cos(angleZ);
        localVertex = Vec3f(
            localVertex.x * cosZ - localVertex.y * sinZ,
            localVertex.x * sinZ + localVertex.y * cosZ,
            localVertex.z
            );

        // Возвращаем вершину в глобальную систему координат
        vertex = localVertex;
    }
}

// Переопределение метода от Object (необходимо для сцены)
double Star:: is_intersect(const Ray& r) const  {
    double minT = -1.0;
    int intersectedPlaneIndex = -1;
    for (size_t i = 0; i < planes.size(); ++i) {
        double t = planes[i].is_intersect(r);
        if (t > 1e-6 && (minT < 0 || t < minT)) {
            minT = t;
            intersectedPlaneIndex = (int)i;
        }
    }
    // Сохраняем индекс пересечённой грани
    lastPlaneIndex = intersectedPlaneIndex;
    return minT;
}

Vec3f Star:: get_normal(const Vec3f& v) const  {
    // Используем сохранённый индекс грани, чтобы получить корректную нормаль
    if (lastPlaneIndex >= 0 && (size_t)lastPlaneIndex < planes.size()) {
        return planes[lastPlaneIndex].get_normal(v).Normilize();
    }
    return Vec3f(0, 0, 0);
}

void Star::tick() {
    // Увеличиваем время
    animTime += 0.02;

    // Зададим колебательное движение по оси Z
    double delta = 50; // амплитуда
    double offsetZ = delta * sin(animTime);

    // Перемещаем звезду по синусоиде вверх-вниз
    // Так как мы в moving меняем center, лучше пусть moving меняет только центр, в итоге без него обошлись)

    Vec3f baseCenter(-50, 100, 250); // начальная точка где звезда была
    center = baseCenter + Vec3f(0,offsetZ,0);

    // Вращение звезды вокруг оси Z (например)
    double rotateSpeed = 0.01; // скорость вращения
    rotating(0, rotateSpeed, 0);

    // После изменения положения и вращения обновляем planes
    updatePlanes();
}


//
// Конструкторы Peaks4
//

Peaks4::Peaks4() : Object(Color()), lastPlaneIndex(-1), scale(1.0), rotationAngle(0.0) {
}

Peaks4::Peaks4(const Vec3f& c, double scaleFactor, Color col, double spect, double refl)
    : Object(col), lastPlaneIndex(-1), scale(scaleFactor), center(c)
{
    s = spect;
    r = refl;
    scale = scaleFactor;
    center = c;

    // Определяем вершины
    vertices = {
                //Левый тэтраэдр
        Vec3f(-50, 0.000000,0),
        Vec3f(-65, 0, -15),
        Vec3f(-65, 0, 15),
        Vec3f(-65, 50, 0),
        //Дальний тэтраэдр
        Vec3f(0, 0, 50),
        Vec3f(-15, 0, 65),
        Vec3f(15, 0, 65),
        Vec3f(0.000000, 50, 65),
        //Правый тэтраэдр
        Vec3f(50, 0.000000, 0.000000),
        Vec3f(65, 0, 15),
        Vec3f(65, 0, -15),
        Vec3f(65, 50, 0),
        //Ближний тэтраэдр
        Vec3f(0, 0, -50),
        Vec3f(15, 0, -65),
        Vec3f(-15, 0, -65),
        Vec3f(0, 50, -65)
    };

    // Определяем грани (треугольники)
    planes = {
              //Левый тэтраэдр
        Plane3v(vertices[0]* scale   + center, vertices[1]* scale   + center, vertices[3]* scale + center, col),
        Plane3v(vertices[0]* scale   + center, vertices[2]* scale  + center, vertices[3]* scale  + center, col),
        Plane3v(vertices[1]* scale   + center, vertices[2]* scale  + center, vertices[3]* scale  + center, col),
        Plane3v(vertices[0]* scale   + center, vertices[1]* scale  + center, vertices[2]* scale   + center, col),
        //Дальний тэтраэдр
        Plane3v(vertices[4]* scale   + center, vertices[5]* scale   + center, vertices[7]* scale   + center, col),
        Plane3v(vertices[5]* scale   + center, vertices[6]* scale   + center, vertices[7]* scale   + center, col),
        Plane3v(vertices[4]* scale   + center, vertices[6]* scale   + center, vertices[7]* scale   + center, col),
        Plane3v(vertices[4]* scale   + center, vertices[5]* scale   + center, vertices[6]* scale   + center, col),
        //Правый тэтраэдр
        Plane3v(vertices[8]* scale   + center, vertices[10]* scale   + center, vertices[11]* scale   + center, col),
        Plane3v(vertices[8]* scale   + center, vertices[9]* scale   + center, vertices[11]* scale   + center, col),
        Plane3v(vertices[9]* scale   + center, vertices[10]* scale   + center, vertices[11]* scale   + center, col),
        Plane3v(vertices[8]* scale   + center, vertices[9]* scale   + center, vertices[10]* scale   + center, col),
        //Ближний тэтраэдр
        Plane3v(vertices[12]* scale   + center, vertices[13]* scale   + center, vertices[15]* scale   + center, col),
        Plane3v(vertices[12]* scale   + center, vertices[14]* scale   + center, vertices[15]* scale   + center, col),
        Plane3v(vertices[13]* scale   + center, vertices[14]* scale   + center, vertices[15]* scale   + center, col),
        Plane3v(vertices[12]* scale   + center, vertices[13]* scale   + center, vertices[14]* scale   + center, col)
    };

}


//
// Функции Peaks4
//

double Peaks4:: is_intersect(const Ray& r) const  {
    double minT = -1.0;
    int intersectedPlaneIndex = -1;
    for (size_t i = 0; i < planes.size(); ++i) {
        double t = planes[i].is_intersect(r);
        if (t > 1e-6 && (minT < 0 || t < minT)) {
            minT = t;
            intersectedPlaneIndex = (int)i;
        }
    }
    // Сохраняем индекс пересечённой грани
    lastPlaneIndex = intersectedPlaneIndex;
    return minT;
}

Vec3f Peaks4:: get_normal(const Vec3f& v) const  {
    // Используем сохранённый индекс грани, чтобы получить корректную нормаль
    if (lastPlaneIndex >= 0 && (size_t)lastPlaneIndex < planes.size()) {
        return planes[lastPlaneIndex].get_normal(v).Normilize();
    }
    return Vec3f(0, 0, 0);
}

void Peaks4::updatePlanes() {
    planes.clear();
    planes = {
        //Левый тэтраэдр
        Plane3v(vertices[0]* scale   + center, vertices[1]* scale   + center, vertices[3]* scale + center, color),
        Plane3v(vertices[0]* scale   + center, vertices[2]* scale  + center, vertices[3]* scale  + center, color),
        Plane3v(vertices[1]* scale   + center, vertices[2]* scale  + center, vertices[3]* scale  + center, color),
        Plane3v(vertices[0]* scale   + center, vertices[1]* scale  + center, vertices[2]* scale   + center, color),
        //Дальний тэтраэдр
        Plane3v(vertices[4]* scale   + center, vertices[5]* scale   + center, vertices[7]* scale   + center, color),
        Plane3v(vertices[5]* scale   + center, vertices[6]* scale   + center, vertices[7]* scale   + center, color),
        Plane3v(vertices[4]* scale   + center, vertices[6]* scale   + center, vertices[7]* scale   + center, color),
        Plane3v(vertices[4]* scale   + center, vertices[5]* scale   + center, vertices[6]* scale   + center, color),
        //Правый тэтраэдр
        Plane3v(vertices[8]* scale   + center, vertices[10]* scale   + center, vertices[11]* scale   + center, color),
        Plane3v(vertices[8]* scale   + center, vertices[9]* scale   + center, vertices[11]* scale   + center, color),
        Plane3v(vertices[9]* scale   + center, vertices[10]* scale   + center, vertices[11]* scale   + center, color),
        Plane3v(vertices[8]* scale   + center, vertices[9]* scale   + center, vertices[10]* scale   + center, color),
        //Ближний тэтраэдр
        Plane3v(vertices[12]* scale   + center, vertices[13]* scale   + center, vertices[15]* scale   + center, color),
        Plane3v(vertices[12]* scale   + center, vertices[14]* scale   + center, vertices[15]* scale   + center, color),
        Plane3v(vertices[13]* scale   + center, vertices[14]* scale   + center, vertices[15]* scale   + center, color),
        Plane3v(vertices[12]* scale   + center, vertices[13]* scale   + center, vertices[14]* scale   + center, color)
    };

}

// Перемещение центра звезды
void Peaks4::moving(const Vec3f& delta) {
    for (auto& vertex : vertices) {
        vertex = vertex + delta;
    }
    center = center + delta;
}

// Масштабирование звезды
void Peaks4::scaling(double factor) {
    for (auto& vertex : vertices) {
        vertex = center + (vertex - center) * factor;
    }
}

// Вращение вокруг осей
void Peaks4:: rotating(double angleX, double angleY, double angleZ) {
    for (auto& vertex : vertices) {
        // Приводим вершину к локальной системе координат звезды
        Vec3f localVertex = vertex;

        // Вращение вокруг оси X
        double sinX = sin(angleX);
        double cosX = cos(angleX);
        localVertex = Vec3f(
            localVertex.x,
            localVertex.y * cosX - localVertex.z * sinX,
            localVertex.y * sinX + localVertex.z * cosX
            );

        // Вращение вокруг оси Y
        double sinY = sin(angleY);
        double cosY = cos(angleY);
        localVertex = Vec3f(
            localVertex.x * cosY + localVertex.z * sinY,
            localVertex.y,
            -localVertex.x * sinY + localVertex.z * cosY
            );

        // Вращение вокруг оси Z
        double sinZ = sin(angleZ);
        double cosZ = cos(angleZ);
        localVertex = Vec3f(
            localVertex.x * cosZ - localVertex.y * sinZ,
            localVertex.x * sinZ + localVertex.y * cosZ,
            localVertex.z
            );

        // Возвращаем вершину в глобальную систему координат
        vertex = localVertex;
    }
}

