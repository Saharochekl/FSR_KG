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


struct Edge {
    QPointF p1, p2;

    Edge(const QPointF& a, const QPointF& b) : p1(a), p2(b) {}

    bool operator==(const Edge& other) const {
        return ((p1 == other.p1 && p2 == other.p2) ||
                (p1 == other.p2 && p2 == other.p1));
    }
};

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

int orientation(QPointF p, QPointF q, QPointF r);

bool onSegment(QPointF p, QPointF q, QPointF r);
bool doIntersect(QPointF A, QPointF B, QPointF C, QPointF D, QPointF& intersection);
bool isConvex(const QVector<QPointF> &polygon);

QVector<QPointF> jarvisConvexHull( QVector<QPointF> &points);
QVector<Edge> getTriangulation(const QVector<QPointF> &points, QStringList &logMessages);

QVector<Triangle> delaunayTriangulation(const QVector<QPointF>& points);

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
