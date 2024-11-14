#include "mathfunctions.h"
#include <algorithm>
#include <iostream>
#include "longdouble.h"
#include <QVector>
#include <QPointF>
#include <queue>
#include <set>



using std::queue;
using std::set;

QVector<QPair<QPointF, QPointF>> getTriangulation(QVector<QPointF> &points) {
    QVector<QPair<QPointF, QPointF>> edges; // Вектор рёбер триангуляции
    if (points.size() < 3) {
        return edges; // Триангуляция невозможна, если меньше 3 точек
    }

    // Сортируем точки по y и x координатам
    std::sort(points.begin(), points.end(), [](QPointF a, QPointF b) {
        return (a.y() < b.y()) || ((a.y() == b.y()) && (a.x() < b.x()));
    });

    // Сортируем оставшиеся точки по полярному углу
    std::sort(points.begin() + 1, points.end(), [&](QPointF a, QPointF b) {
        double angleA = atan2(a.y() - points[0].y(), a.x() - points[0].x());
        double angleB = atan2(b.y() - points[0].y(), b.x() - points[0].x());
        return angleA < angleB;
    });

    queue<QPair<QPointF, QPointF>> processQueue;
    std::set<QPair<QPointF, QPointF>, QPointFPairComparator> seenEdges;


    processQueue.push(qMakePair(points[0], points[1]));
    seenEdges.insert(qMakePair(points[0], points[1]));

    while (!processQueue.empty()) {
        QPair<QPointF, QPointF> edge = processQueue.front();
        processQueue.pop();

        // Поиск наиболее удаленной точки от текущего ребра
        QPointF maxPoint;
        double maxDistance = -1;

        for (auto &point : points) {
            // Пропускаем точки, которые являются концами текущего ребра
            if (point == edge.first || point == edge.second) {
                continue;
            }

            // Вычисляем расстояние от точки до текущего ребра
            double distance = qAbs((edge.second.x() - edge.first.x()) * (edge.first.y() - point.y()) -
                                   (edge.first.x() - point.x()) * (edge.second.y() - edge.first.y())) /
                              sqrt(pow(edge.second.x() - edge.first.x(), 2) + pow(edge.second.y() - edge.first.y(), 2));

            if (distance > maxDistance) {
                maxPoint = point;
                maxDistance = distance;
            }
        }

        // Добавляем рёбра к результату
        if (maxDistance > 0) {
            QPair<QPointF, QPointF> edge1 = qMakePair(edge.first, maxPoint);
            QPair<QPointF, QPointF> edge2 = qMakePair(edge.second, maxPoint);

            // Добавляем новое ребро, если оно еще не было обработано
            if (seenEdges.find(edge1) == seenEdges.end()) {
                processQueue.push(edge1);
                seenEdges.insert(edge1);
            }
            if (seenEdges.find(edge2) == seenEdges.end()) {
                processQueue.push(edge2);
                seenEdges.insert(edge2);
            }

            // Добавляем текущее ребро в список рёбер триангуляции
            edges.append(edge1);
            edges.append(edge2);
        }
        // Добавляем исходное ребро, если оно еще не было добавлено
        if (seenEdges.find(edge) == seenEdges.end()) {
            edges.append(edge);
            seenEdges.insert(edge);
        }
    }

    return edges;
}


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

QVector<QPointF> jarvisConvexHull( QVector<QPointF> &points) {
    QVector<QPointF> hull;
    if (points.size() < 3) {
        return hull; // Выпуклая оболочка невозможна, если меньше 3 точек
    }

    // Находим самую левую точку
    int l = 0;
    for (int i = 1; i < points.size(); i++) {
        if (points[i].x() < points[l].x()) {
            l = i;
        }
    }

    int p = l, q;
    do {
        // Добавляем текущую точку в оболочку
        hull.push_back(points[p]);

        // Находим следующую точку, которая образует левый поворот
        q = (p + 1) % points.size();
        for (int i = 0; i < points.size(); i++) {
            if (orientation(points[p], points[i], points[q]) == 2) {
                q = i;
            }
        }

        // Переходим к следующей точке
        p = q;

    } while (p != l); // Пока не вернулись к начальной точке

    return hull;
}


