#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H
#include <QPointF>
#include <math.h>
#include <QPoint>

double vectorProduct(QPointF p1, QPointF p2);
double distance(QPointF p1, QPointF p2);
double norma(QPointF vec);
double angleBetwenDeg(QPointF p1, QPointF p2, QPointF d1, QPointF d2);
int orientation(QPointF p, QPointF q, QPointF r);
bool onSegment(QPointF p, QPointF q, QPointF r);
bool doIntersect(QPointF A, QPointF B, QPointF C, QPointF D, QPointF& intersection);
QVector<QPointF> jarvisConvexHull( QVector<QPointF> &points);
QVector<QPair<QPointF, QPointF>> getTriangulation(QVector<QPointF> &points);
// Компаратор для сравнения пар точек QPointF
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
