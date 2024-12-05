#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H
#include <QPointF>
#include <math.h>
#include <QPoint>
#include <cmath>
#include <vector>
#include <QVector>
#include <QSet>
#include <limits>
#include "QtGui/qpolygon.h"
#include "clipper.hpp"
using namespace ClipperLib;


struct Edge {
    QPointF p1, p2;
    int pred; // для пересечения

    Edge() : pred(0) {}
    Edge(const QPointF& a, const QPointF& b) : p1(a), p2(b), pred(0) {}

    bool operator==(const Edge& other) const {
        return ((p1 == other.p1 && p2 == other.p2) ||
                (p1 == other.p2 && p2 == other.p1));
    }
};


// Статическая функция для вычисления расстояния между двумя точками
inline double dist(const QPointF &A, const QPointF &B) {
    double dx = A.x() - B.x();
    double dy = A.y() - B.y();
    return std::sqrt(dx*dx + dy*dy);
}

inline uint qHash(const QPointF &key, uint seed = 0) {
    // Используем масштабирование для сохранения точности
    qint64 x = qRound64(key.x() * 1000000);
    qint64 y = qRound64(key.y() * 1000000);
    return qHash(x, seed) ^ qHash(y, seed);
}

inline uint qHash(const Edge &key, uint seed = 0) {
    return qHash(key.p1, seed) ^ qHash(key.p2, seed);
}

struct Triangle {
    QPointF p1, p2, p3;
    QPointF circumcenter;
    double radiusSquared;
    Triangle() : p1(), p2(), p3(), circumcenter(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()),
        radiusSquared(std::numeric_limits<double>::infinity()) {}
    Triangle(const QPointF& a, const QPointF& b, const QPointF& c) : p1(a), p2(b), p3(c) {
        calculateCircumcircle();
    }
    // Добавляем оператор ==
    bool operator==(const Triangle& other) const {
        // Сравниваем треугольники по их вершинам
        // Поскольку порядок вершин может быть разным, нужно проверить все возможные перестановки
        QVector<QPointF> thisPoints = {p1, p2, p3};
        QVector<QPointF> otherPoints = {other.p1, other.p2, other.p3};

        // Сортируем точки для корректного сравнения
        std::sort(thisPoints.begin(), thisPoints.end(), [](const QPointF& a, const QPointF& b) {
            return (a.x() < b.x()) || (a.x() == b.x() && a.y() < b.y());
        });
        std::sort(otherPoints.begin(), otherPoints.end(), [](const QPointF& a, const QPointF& b) {
            return (a.x() < b.x()) || (a.x() == b.x() && a.y() < b.y());
        });

        return thisPoints[0] == otherPoints[0] &&
               thisPoints[1] == otherPoints[1] &&
               thisPoints[2] == otherPoints[2];
    }
    void calculateCircumcircle();
    bool containsPoint(const QPointF& p) const;
};

double vectorProduct(QPointF p1, QPointF p2);
double distance(QPointF p1, QPointF p2);
double norma(QPointF vec);
double angleBetwenDeg(QPointF p1, QPointF p2, QPointF d1, QPointF d2);
double angle(QPointF A, QPointF B, QPointF C);

int orientation(QPointF p, QPointF q, QPointF r);
int pred(Edge s, QPointF c); //Предикат для пересечений, по факту костыль тот ещё
int per(Edge seg1, Edge seg2); //Пересечение отрезков...
int per_polygon(QVector <Edge> seg, QPointF A, QPointF B);

bool onSegment(QPointF p, QPointF q, QPointF r);
bool doIntersect(QPointF A, QPointF B, QPointF C, QPointF D, QPointF& intersection);
bool in_figure(QVector <Edge> seg, QPointF A);
bool isConvex(const QVector<QPointF> &polygon);
bool check(Edge s0, QVector <Edge> s); //проверяем, записано ли уже ребро
bool seg_in_figure(QVector <Edge> seg, Edge s);
bool point_not_in_vec(QVector <QPointF> p, QPointF A);


QPointF nextpoint(QPointF A, QPointF B, int N, QVector <QPointF> mas);
QPointF per_point(Edge seg1, Edge seg2);

QVector<Edge> getTriangulation(const QVector<QPointF> &points, QStringList &logMessages);
QVector <Edge> do_polygon(QVector <Edge> seg1);
QVector <Edge> do_intersection(QVector <Edge> segA, QVector <Edge> segB);
QVector <Edge> do_union(QVector <Edge> segA, QVector <Edge> segB);
QVector <Edge> do_difference(QVector <Edge> segA, QVector <Edge> segB);
QVector <Edge> do_differenceSem(QVector <Edge> segA, QVector <Edge> segB);

QVector<Triangle> delaunayTriangulation(const QVector<QPointF>& points);
QVector <Triangle> do_triang(QVector <QPointF> mas);
QVector <Triangle> do_triang_poligon(QVector <Edge> seg);
QVector <Triangle> do_intersection_trisngl(QVector <Edge> segA, QVector <Edge> segB);

QVector<QPointF> jarvisConvexHull( QVector<QPointF> &points);
QVector<QPointF> intersectConvexPolygons(const QVector<QPointF> &poly1, const QVector<QPointF> &poly2);
QVector<QPointF> differenceConvexPolygons(const QVector<QPointF> &poly1, const QVector<QPointF> &poly2);
QVector<QPointF> combiningConvexPolygons(const QVector<QPointF> &poly1, const QVector<QPointF> &poly2);


Path qPolygonFToPath(const QPolygonF &polygon);

// Функция для преобразования ClipperLib::Paths в QVector<QPolygonF>
QVector<QPolygonF> pathsToQPolygons(const Paths &paths);
QVector<QPolygonF> computeIntersection(const QPolygonF &poly1, const QPolygonF &poly2);
QVector<QPolygonF> computeUnion(const QPolygonF &poly1, const QPolygonF &poly2);
QVector<QPolygonF> computeDifference(const QPolygonF &poly1, const QPolygonF &poly2);


//Компаратор для сравнения пар точек QPointF
struct QPointFPairComparator {
    bool operator()(const QPair<QPointF, QPointF> &lhs, const QPair<QPointF, QPointF> &rhs) const {
        if (lhs.first.x() != rhs.first.x())
            return lhs.first.x() < rhs.first.x();
        if (lhs.first.y() != rhs.first.y())
            return lhs.first.y() < rhs.first.y();
        if (lhs.second.x() != rhs.second.x())
            return lhs.second.x() < rhs.second.x();
        return lhs.second.y() < rhs.second.y();
    }
};


#endif // MATHFUNCTIONS_H
