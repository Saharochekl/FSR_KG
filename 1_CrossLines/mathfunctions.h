#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H
#include <QPointF>
#include <math.h>
#include <QPoint>
#include <cmath>
#include <vector>


double vectorProduct(QPointF p1, QPointF p2);
double distance(QPointF p1, QPointF p2);
double norma(QPointF vec);
double angleBetwenDeg(QPointF p1, QPointF p2, QPointF d1, QPointF d2);
int orientation(QPointF p, QPointF q, QPointF r);
bool onSegment(QPointF p, QPointF q, QPointF r);
bool doIntersect(QPointF A, QPointF B, QPointF C, QPointF D, QPointF& intersection);
QVector<QPointF> jarvisConvexHull( QVector<QPointF> &points);
//QVector<QPair<QPointF, QPointF>> getTriangulation(QVector<QPointF> &points, QStringList &logMessages);
// Компаратор для сравнения пар точек QPointF
//struct QPointFPairComparator {
//    bool operator()(const QPair<QPointF, QPointF> &lhs, const QPair<QPointF, QPointF> &rhs) const {
//        if (lhs.first.x() != rhs.first.x())
//            return lhs.first.x() < rhs.first.x();
//        if (lhs.first.y() != rhs.first.y())
//            return lhs.first.y() < rhs.first.y();
//        if (lhs.second.x() != rhs.second.x())
//            return lhs.second.x() < rhs.second.x();
//        return lhs.second.y() < rhs.second.y();
//    }
//};

// Шаблон функции для возвращения знака числа
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

// Определение структуры точки с оператором вычитания, сравнения и конструктором
struct point {
    long double x, y;
    point operator-(point obj) { return point(this->x - obj.x, this->y - obj.y); }
    bool operator==(point obj) { return (this->x == obj.x) && (this->y == obj.y); }
    point(long double a, long double b) : x(a), y(b) {}
    static long double dist(point a, point b) {
        return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
    }
};

// Определение структуры тройки точек с конструктором
struct triplet {
    point a, b, c;
    triplet(point p1, point p2, point p3) : a(p1), b(p2), c(p3) {}
};

// Определение структуры ребра с конструктором и методами
struct edge {
    point a, b;
    long double vect_x, vect_y;
    edge(point p1, point p2) : a(p1), b(p2) {
        auto buf = b - a;
        vect_x = buf.x;
        vect_y = buf.y;
    }
    long double operator*(edge obj) {
        return this->vect_x * obj.vect_x + this->vect_y * obj.vect_y;
    }
    long double norm() { return point::dist(this->a, this->b); }
    bool operator==(edge obj) {
        return (this->a == obj.a && this->b == obj.b);
    }
    long double edge_vect(edge target) {
        return sgn(this->vect_x * target.vect_y - this->vect_y * target.vect_x);
    }
    long double edge_pol(point target) {
        auto target_buf_a = edge(this->a, target);
        auto target_buf_b = edge(this->b, target);
        return this->edge_vect(target_buf_a) * std::acos((std::pow(target_buf_a.norm(), 2) + std::pow(target_buf_b.norm(), 2) - std::pow(this->norm(), 2)) / (2 * target_buf_a.norm() * target_buf_b.norm()));
    }
};

// Конвертация между QPointF и point
point toPoint(const QPointF &p) {
    return point(p.x(), p.y());
}

QPointF toQPointF(const point &p) {
    return QPointF(p.x, p.y);
}

// Прототип функции для триангуляции
std::vector<edge> getTriangulation(std::vector<point> &data, std::vector<triplet> &triplet_base);


#endif // MATHFUNCTIONS_H
