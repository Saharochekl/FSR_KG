#include "mathfunctions.h"
#include <algorithm>
#include <iostream>
#include "longdouble.h"

int orientation(QPointF p, QPointF q, QPointF r) {

    LongDouble pqx = LongDouble(q.x()) - LongDouble(p.x());
    LongDouble pry = LongDouble(r.y()) - LongDouble(p.y());
    LongDouble pqy = LongDouble(q.y()) - LongDouble(p.y());
    LongDouble prx = LongDouble(r.x()) - LongDouble(p.x());

    LongDouble val = (pqx * pry) - (pqy * prx);
    if (val == LongDouble(0)) {
        return 0;  // На прямой
    } else if (val > LongDouble(0)) {
        return 1;  // Правый поворот
    } else {
        return 2;  // Левый поворот
    }
}

bool onSegment(QPointF p, QPointF q, QPointF r) {
    return ((q.x() <= std::max(p.x(), r.x())) && (q.x() >= std::min(p.x(), r.x()))&&
            (q.y() <= std::max(p.y(), r.y())) && (q.y() >= std::min(p.y(), r.y())));
}

bool doIntersect(QPointF A, QPointF B, QPointF C, QPointF D, QPointF& intersection)
{    // Вычисляем ориентации для четырёх комбинаций точек
    int o1 = orientation(A, B, C);
    int o2 = orientation(A, B, D);
    int o3 = orientation(C, D, A);
    int o4 = orientation(C, D, B);

    // Основное условие: отрезки пересекаются, если ориентации различны
    if (o1 != o2 && o3 != o4) {
        // Вычисляем точку пересечения
        double a1 = B.y() - A.y();
        double b1 = A.x() - B.x();
        double c1 = a1 * A.x() + b1 * A.y();

        double a2 = D.y() - C.y();
        double b2 = C.x() - D.x();
        double c2 = a2 * C.x() + b2 * C.y();

        double determinant = a1 * b2 - a2 * b1;

        if (determinant != 0) {
            double x = (b2 * c1 - b1 * c2) / determinant;
            double y = (a1 * c2 - a2 * c1) / determinant;
            intersection = QPointF(x, y);

            // Дополнительно проверяем, лежит ли точка пересечения на обоих отрезках
            if (onSegment(A, intersection.toPoint(), B) && onSegment(C, intersection.toPoint(), D)) {
                return true;
            }
        }
        return false;
    }

    // Граничные случаи: когда точки лежат на одном отрезке
    if (o1 == 0 && onSegment(A, C, B)) {
        intersection = QPointF(C);
        return true;
    }
    if (o2 == 0 && onSegment(A, D, B)) {
        intersection = QPointF(D);
        return true;
    }
    if (o3 == 0 && onSegment(C, A, D)) {
        intersection = QPointF(A);
        return true;
    }
    if (o4 == 0 && onSegment(C, B, D)) {
        intersection = QPointF(B);
        return true;
    }

    return false;  // Отрезки не пересекаются
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


