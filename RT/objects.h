#ifndef OBJECTS_H
#define OBJECTS_H

#include <math.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

struct Vec3f
{
    double x;
    double y;
    double z;

    Vec3f() { }
    Vec3f(double inX, double inY, double inZ) : x(inX), y(inY), z(inZ) { }

    Vec3f operator+(const Vec3f& o) const
    {
        return Vec3f(x + o.x, y + o.y, z + o.z);
    }

    Vec3f operator-(const Vec3f& o) const
    {
        return Vec3f(x - o.x, y - o.y, z - o.z);
    }

    Vec3f operator*(const double& d) const
    {
        return Vec3f(x * d, y * d, z * d);
    }

    double operator*(const Vec3f& o) const
    {
        return x * o.x + y * o.y + z * o.z;
    }

    double sq() const
    {
        return x * x + y * y + z * z;
    }

    double length() const
    {
        return sqrt(sq());
    }

    Vec3f operator%(const Vec3f& o) const
    {
        return Vec3f(y * o.z - z * o.y, -x * o.z + z * o.x, x * o.y - y * o.x);
    }
    Vec3f norm(){
        return (*this)*(1/length());
    }

    Vec3f Normilize() const { return (*this) * (1.0 / length()); }
};

inline Vec3f operator*(const double& a, const Vec3f& v)
{
    return v * a;
}

inline double mixed(const Vec3f& a, const Vec3f& b, const Vec3f& c)
{
    return (a % b) * c;
}

class Projector
{
protected:
    double l0; // расстояние до картинной плоскости

public:
    Projector() : l0(1) { }
    Projector(double d) : l0(d) { }

    Vec3f operator()(const Vec3f& v) const
    {
        double s = 1.0 / (1.0 + v.z / l0);
        return Vec3f(s * v.x, s * v.y, s * v.z);
    }

    double get_l0() const
    {
        return l0;
    }
};

struct Color
{
    unsigned short R;
    unsigned short G;
    unsigned short B;

    Color() : R(0), G(0), B(0) { }
    Color(unsigned short iR,
          unsigned short iG,
          unsigned short iB) : R(iR), G(iG), B(iB) { }

    Color operator*(const double& d) const
    {
        return Color(d * R, d * G, d * B);
    }

    Color operator+(const Color& c) const
    {
        return Color(R + c.R, G + c.G,  B + c.B);
    }

    Color operator-(const Color& c) const
    {
        return Color(R - c.R, G - c.G,  B - c.B);
    }

};

inline Color operator*(const double& d, const Color& c)
{
    return c * d;
}

struct Ray
{
    Vec3f beg;
    Vec3f dir;

    Ray() { }
    Ray(const Vec3f& b, const Vec3f& d) : beg(b), dir(d) { }

    Vec3f operator()(double t) const
    {
        return beg + t * dir;
    }
};

enum { AMBIENT, POINT, PARALLEL };

struct LightSource
{
    int type;
    Vec3f position;
    double intensity;

    LightSource() { }
    LightSource(int t, const Vec3f& v, const double& intens)
        : type(t), position(v), intensity(intens) { }
};

struct Object
{
    Color color;
    double s;// приломление
    double r;// отражение
    virtual double is_intersect(const Ray& r) const = 0;
    virtual Vec3f get_normal(const Vec3f& v) const = 0;

    Object() { }
    Object(Color c) : color(c) { }
    virtual void tick() { }
};

inline double is_intersect_tr(const Ray& ray,
                              const Vec3f& A,
                              const Vec3f& B,
                              const Vec3f& C)
{
    Vec3f AB = B - A;
    Vec3f AC = C - A;
    Vec3f AO = ray.beg - A;

    double delta = mixed(AB, AC, ray.dir);
    double u = mixed(AO, AC, ray.dir) / delta;
    double v = mixed(AB, AO, ray.dir) / delta;
    double t = -mixed(AB, AC, AO) / delta;

    if(u >= 0 && u <= 1 && v >= 0 && v <= 1 && u + v <= 1)
        return t;

    return -1.0;
}

struct Sphere : public Object
{

    double radius;
    Vec3f center;

    Vec3f dir;
    double step;
    double phase;
    double amp;

    Sphere() { }
    Sphere(const double& rad, const Vec3f& v) : radius(rad), center(v), dir(0, 1, 0), step(5), phase(0), amp(300) { }
    Sphere(const double& rad,
           const Vec3f& v,
           Color c) : Object(c), radius(rad), center(v), dir(0, 1, 0), step(5), phase(0), amp(300) {r=-1; s=-1;}
    Sphere(const double& rad,
           const Vec3f& v,
           Color c,  double spect, double refl) : Object(c), radius(rad), center(v), dir(0, 1, 0), step(5), phase(0), amp(300) {r = refl; s = spect;}

    Sphere(const double& rad,
           const Vec3f& v,
           Color c,
           double in_step,
           Vec3f in_dir,
           double in_amp) :
        Object(c), radius(rad), center(v),
        dir(in_dir), step(in_step), phase(0), amp(in_amp) { }

    virtual Vec3f get_normal(const Vec3f& v) const
    {
        return (v - center).Normilize();
    }

    Vec3f gep_pos() const
    {
        return center + phase * dir;
    }

    virtual double is_intersect(const Ray& r) const
    {
        double A = r.dir.sq();
        Vec3f v = r.beg - gep_pos();
        double B = r.dir * v;
        double C = v.sq() - radius * radius;

        double D = B * B - A * C;

        if(D < 0)
            return -1.0;

        double sD = sqrt(D);
        double t1 = (-B - sD) / A;
        double t2 = (-B + sD) / A;

        return (t1 > 0) ? t1 : t2;
    }

    virtual void tick()
    {
        phase += step;
        if(fabs(phase) > amp)
        {
            step *= -1;
            phase += step * 2;
        }
    }
};

struct Plane3v : public Object
{
    Vec3f vert[3];

    Plane3v() : Object(Color()) { } // dummy object
    Plane3v(const Vec3f& A,
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
    Plane3v(const Vec3f& A,
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


    virtual double is_intersect(const Ray& r) const
    {
        return is_intersect_tr(r, vert[0], vert[1], vert[2]);

    }

    virtual Vec3f get_normal(const Vec3f& v) const
    {
        return ((vert[2] - vert[0]) % (vert[1] - vert[0])).Normilize();
    }
};

struct Plane4v : public Object
{
    Vec3f vert[4];

    Plane4v() : Object(Color()) { } // dummy object
    Plane4v(const Vec3f& A,
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
    Plane4v(const Vec3f& A,
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


    virtual double is_intersect(const Ray& r) const
    {
        double t1 = is_intersect_tr(r, vert[0], vert[1], vert[2]);
        if(t1 > 0)
            return t1;

        return is_intersect_tr(r, vert[2], vert[3], vert[0]);
    }

    virtual Vec3f get_normal(const Vec3f& v) const
    {
        return ((vert[2] - vert[0]) % (vert[1] - vert[0])).Normilize();
    }
};

struct Star : public Object{
    std::vector<Plane3v> planes; // Грани звезды
    std::vector<Vec3f> vertices;   // Вершины
    Vec3f center;               // Центр звезды
    double scale;               // Масштаб звезды
    double rotationAngle;       // Угол вращения вокруг оси Z
    Star(): Object(Color()) { } // dummy object
    Star(const Vec3f& c, double s, Color col)
        :  Object(col) {
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
            Plane3v(vertices[6]* s + c, vertices[10]* s + c, vertices[0]* s + c, col),
            Plane3v(vertices[6]* s + c, vertices[1]* s + c, vertices[10]* s + c, col),
            Plane3v(vertices[3]* s + c, vertices[10]* s + c, vertices[5]* s + c, col),
            Plane3v(vertices[5]* s + c, vertices[10]* s + c, vertices[1]* s + c, col),
            Plane3v(vertices[9]* s + c, vertices[4]* s + c, vertices[2]* s + c, col),
            Plane3v(vertices[2]* s + c, vertices[7]* s + c, vertices[9]* s + c, col),
            Plane3v(vertices[7]* s + c, vertices[3]* s + c, vertices[9]* s + c, col),
            Plane3v(vertices[3]* s + c, vertices[5]* s + c, vertices[9]* s + c, col),
            Plane3v(vertices[5]* s + c, vertices[1]* s + c, vertices[9]* s + c, col),
            Plane3v(vertices[9]* s + c, vertices[6]* s + c, vertices[0]* s + c, col),
            Plane3v(vertices[6]* s + c, vertices[9]* s + c, vertices[1]* s + c, col),
            Plane3v(vertices[4]* s + c, vertices[9]* s + c, vertices[0]* s + c, col),
            Plane3v(vertices[7]* s + c, vertices[10]* s + c, vertices[3]* s + c, col),
            Plane3v(vertices[2]* s + c, vertices[10]* s + c, vertices[7]* s + c, col),
            Plane3v(vertices[4]* s + c, vertices[10]* s + c, vertices[2]* s + c, col),
            Plane3v(vertices[0]* s + c, vertices[10]* s + c, vertices[4]* s + c, col)
        };

    }
    // Переопределение метода от Object (необходимо для сцены)
    virtual double is_intersect(const Ray& r) const override {
        double minT = -1.0;
        int intersectedPlaneIndex = -1;
        for (size_t i = 0; i < planes.size(); ++i) {
            double t = planes[i].is_intersect(r);
            if (t > 0 && (minT < 0 || t < minT)) {
                minT = t;
                intersectedPlaneIndex = i; // Сохраняем индекс пересечённой грани
                std::cout << "Ray: (" << r.beg.x << ", " << r.beg.y << ", " << r.beg.z
                          << ") -> (" << r.dir.x << ", " << r.dir.y << ", " << r.dir.z << ")\n";
                std::cout << "Nearest intersection at t = " << minT
                          << ", with plane index: " << intersectedPlaneIndex << "\n";
            }

        }

        return minT;
    }

    virtual Vec3f get_normal(const Vec3f& v) const override {
        for (const auto& plane : planes) {
            if (plane.is_intersect(Ray(v, Vec3f(0, 0, 1))) > 0) {
                return plane.get_normal(v).Normilize();
            }
        }
        return Vec3f(0, 0, 0); // Если точка не принадлежит ни одной грани
    }

    // Перемещение центра звезды
    void moving(const Vec3f& delta) {
        for (auto& vertex : vertices) {
            vertex = vertex + delta;
        }
        center = center + delta;
    }

    // Масштабирование звезды
    void scaling(double factor) {
        for (auto& vertex : vertices) {
            vertex = center + (vertex - center) * factor;
        }
    }

    // Вращение вокруг осей
    void rotating(double angleX, double angleY, double angleZ) {
        for (auto& vertex : vertices) {
            // Приводим вершину к локальной системе координат звезды
            Vec3f localVertex = vertex - center;

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
            vertex = center + localVertex;
        }
    }

    // Вывод состояния звезды
    void printState() const {
        std::cout << "Center: (" << center.x << ", " << center.y << ", " << center.z << ")\n";
        std::cout << "Vertices:\n";
        for (const auto& vertex : vertices) {
            std::cout << "  (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")\n";
        }
    }
};

#endif // OBJECTS_H
