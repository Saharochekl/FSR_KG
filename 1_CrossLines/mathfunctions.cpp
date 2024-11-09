#include "mathfunctions.h"
#include <algorithm>

int orientation(QPoint p, QPoint q, QPoint r) {
    int val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());

    if (val == 0) {
        return 0;  // Коллинеарные точки
    } else if (val > 0) {
        return 1;  // Правый поворот
    } else {
        return 2;  // Левый поворот
    }
}

bool onSegment(QPoint p, QPoint q, QPoint r) {
    return (q.x() <= std::max(p.x(), r.x()) && q.x() >= std::min(p.x(), r.x()) &&
            q.y() <= std::max(p.y(), r.y()) && q.y() >= std::min(p.y(), r.y()));
}

bool doIntersect(QPoint A, QPoint B, QPoint C, QPoint D) {
    int o1 = orientation(A, B, C);
    int o2 = orientation(A, B, D);
    int o3 = orientation(C, D, A);
    int o4 = orientation(C, D, B);

    // Основное условие: отрезки пересекаются, если ориентации различны
    if (o1 != o2 && o3 != o4) {
        return true;
    }

    // Граничные случаи: когда точки коллинеарны и лежат на одном отрезке
    if (o1 == 0 && onSegment(A, C, B)) {
        return true;
    }
    if (o2 == 0 && onSegment(A, D, B)) {
        return true;
    }
    if (o3 == 0 && onSegment(C, A, D)) {
        return true;
    }
    if (o4 == 0 && onSegment(C, B, D)) {
        return true;
    }

    return false;
}

double vectorProduct(QPointF p1, QPointF p2)
{
    double res = p1.x() * p2.x() + p1.y() * p2.y();
    return res;
}

double distance(QPointF p1, QPointF p2)
{
 double dist_2 = (p1.x() - p2.x())*(p1.x() - p2.x()) + (p1.y() - p2.y())*(p1.y() - p2.y());
 return sqrt(dist_2);
}

double norma(QPointF vec)
{
    return sqrt(vec.x() * vec.x() + vec.y() * vec.y());
}

double angleBetwenDeg(QPointF p1, QPointF p2, QPointF d1, QPointF d2)
{
    QPointF vec1 = p2 - p1;
    QPointF vec2 = d2 - d1;

    double norma_vec1 = norma(vec1);
    double norma_vec2 = norma(vec2);

    double cos_a = vectorProduct(vec1, vec2) / (norma_vec1 * norma_vec2);

    return acos(cos_a) * 180 / M_PI;
}


