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

void Triangle::calculateCircumcircle() {
    double ax = p1.x();
    double ay = p1.y();
    double bx = p2.x();
    double by = p2.y();
    double cx = p3.x();
    double cy = p3.y();

    double D = 2 * (ax*(by - cy) + bx*(cy - ay) + cx*(ay - by));
    if (D == 0) {
        // Точки коллинеарны
        circumcenter = QPointF(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
        radiusSquared = std::numeric_limits<double>::infinity();
        return;
    }

    double Ux = ((ax*ax + ay*ay)*(by - cy) + (bx*bx + by*by)*(cy - ay) + (cx*cx + cy*cy)*(ay - by)) / D;
    double Uy = ((ax*ax + ay*ay)*(cx - bx) + (bx*bx + by*by)*(ax - cx) + (cx*cx + cy*cy)*(bx - ax)) / D;

    circumcenter = QPointF(Ux, Uy);
    radiusSquared = (Ux - ax)*(Ux - ax) + (Uy - ay)*(Uy - ay);
}

bool Triangle::containsPoint(const QPointF& p) const {
    double dx = p.x() - circumcenter.x();
    double dy = p.y() - circumcenter.y();
    double distSquared = dx*dx + dy*dy;
    return distSquared <= radiusSquared;
}

QVector<Triangle> delaunayTriangulation(const QVector<QPointF>& points) { //Сама триангуляция
    // Создаем супер-треугольник
    double minX = points[0].x();
    double minY = points[0].y();
    double maxX = minX;
    double maxY = minY;

    for (const QPointF& p : points) {
        if (p.x() < minX) minX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() > maxY) maxY = p.y();
    }

    double dx = maxX - minX;
    double dy = maxY - minY;
    double deltaMax = std::max(dx, dy);
    double midx = (minX + maxX) / 2.0;
    double midy = (minY + maxY) / 2.0;

    QPointF p1(midx - 20 * deltaMax, midy - deltaMax);
    QPointF p2(midx, midy + 20 * deltaMax);
    QPointF p3(midx + 20 * deltaMax, midy - deltaMax);

    QVector<Triangle> triangulation;
    triangulation.append(Triangle(p1, p2, p3));

    // Добавим точек в триангуляцию
    for (const QPointF& p : points) {
        QVector<Triangle> badTriangles;
        QVector<Edge> polygon;

        // Ищем лишние треугольники
        for (const Triangle& t : triangulation) {
            if (t.containsPoint(p)) {
                badTriangles.append(t);
            }
        }

        // Ищем границу многоугольника
        QSet<Edge> edgeCount;
        for (const Triangle& t : badTriangles) {
            Edge e1(t.p1, t.p2);
            Edge e2(t.p2, t.p3);
            Edge e3(t.p3, t.p1);

            if (edgeCount.contains(e1)) {
                edgeCount.remove(e1);
            } else {
                edgeCount.insert(e1);
            }

            if (edgeCount.contains(e2)) {
                edgeCount.remove(e2);
            } else {
                edgeCount.insert(e2);
            }

            if (edgeCount.contains(e3)) {
                edgeCount.remove(e3);
            } else {
                edgeCount.insert(e3);
            }
        }

        // Удаляем лишние треугольники
        for (const Triangle& t : badTriangles) {
            triangulation.removeAll(t);
        }

        // Хоба, новые треугольники
        for (const Edge& e : edgeCount) {
            triangulation.append(Triangle(e.p1, e.p2, p));
        }
    }

    // Удаляем треугольники, связанные с супер-треугольником
    QVector<Triangle> finalTriangulation;
    for (const Triangle& t : triangulation) {
        if (t.p1 == p1 || t.p1 == p2 || t.p1 == p3 ||
            t.p2 == p1 || t.p2 == p2 || t.p2 == p3 ||
            t.p3 == p1 || t.p3 == p2 || t.p3 == p3) {
            continue;
        }
        finalTriangulation.append(t);
    }

    return finalTriangulation;
}


QVector<Edge> getTriangulation(const QVector<QPointF> &points, QStringList &logMessages) { //Подготовка и вызов
    QVector<Edge> edges;

    QVector<Triangle> triangles = delaunayTriangulation(points);

    QSet<Edge> edgeSet;
    for (const Triangle &t : triangles) {
        Edge e1(t.p1, t.p2);
        Edge e2(t.p2, t.p3);
        Edge e3(t.p3, t.p1);

        // Добавляем рёбра в набор, чтобы избежать дубликатов
        edgeSet.insert(e1);
        edgeSet.insert(e2);
        edgeSet.insert(e3);
    }

    edges = edgeSet.values().toVector();

    // Логируем рёбра
    for (const Edge &e : edges) {
        QString info = QString("Ребро: (%1, %2) -> (%3, %4)")
                           .arg(e.p1.x())
                           .arg(e.p1.y())
                           .arg(e.p2.x())
                           .arg(e.p2.y());
        logMessages.append(info);
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


