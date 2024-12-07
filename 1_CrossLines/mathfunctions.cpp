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

bool isConvex(const QVector<QPointF> &polygon) {
    int n = polygon.size();
    if (n < 3)
        return false;

    int sign = 0;
    for (int i = 0; i < n; ++i) {
        QPointF p0 = polygon[i];
        QPointF p1 = polygon[(i + 1) % n];
        QPointF p2 = polygon[(i + 2) % n];

        double crossProduct = (p1.x() - p0.x()) * (p2.y() - p1.y()) - (p1.y() - p0.y()) * (p2.x() - p1.x());

        if (crossProduct != 0) {
            int currentSign = (crossProduct > 0) ? 1 : -1;
            if (sign == 0) {
                sign = currentSign;
            } else if (sign != currentSign) {
                return false; // Знак изменился, многоугольник невыпуклый
            }
        }
    }
    return true; // Все знаки одинаковы, многоугольник выпуклый
}

bool Triangle::containsPoint(const QPointF& p) const {
    double dx = p.x() - circumcenter.x();
    double dy = p.y() - circumcenter.y();
    double distSquared = dx*dx + dy*dy;
    return distSquared <= radiusSquared;
}






double distance(QPointF p1, QPointF p2)
{
 double dist_2 = (p1.x() - p2.x())*(p1.x() - p2.x()) + (p1.y() - p2.y())*(p1.y() - p2.y());
 return sqrt(dist_2);
}



// Function to calculate the angle ABC in radians






QVector<QPointF> intersectConvexPolygons(const QVector<QPointF> &poly1, const QVector<QPointF> &poly2) {
    QVector<QPointF> outputList = poly1;

    for (int i = 0; i < poly2.size(); ++i) {
        QPointF A = poly2[i];
        QPointF B = poly2[(i + 1) % poly2.size()];

        QVector<QPointF> inputList = outputList;
        outputList.clear();

        if (inputList.isEmpty())
            break;

        QPointF S = inputList.last();
        for (const QPointF &E : inputList) {
            int E_orientation = orientation(A, B, E);
            int S_orientation = orientation(A, B, S);

            if (E_orientation != 2) {
                if (S_orientation == 2) {
                    QPointF intersectionPoint;
                    doIntersect(S, E, A, B, intersectionPoint);
                    outputList.append(intersectionPoint);
                }
                outputList.append(E);
            } else if (S_orientation != 2) {
                QPointF intersectionPoint;
                doIntersect(S, E, A, B, intersectionPoint);
                outputList.append(intersectionPoint);
            }
            S = E;
        }
    }

    return outputList;
}


QVector<QPointF> differenceConvexPolygons(const QVector<QPointF> &poly1, const QVector<QPointF> &poly2) {
    // Когда-нибудь я это допишу
    QVector<QPointF> intersection = intersectConvexPolygons(poly1, poly2);
    if (intersection.isEmpty()) {
        return poly1;
    } else {
        return QVector<QPointF>(); // Возвращаем пустой результат
    }
}

QVector<QPointF> combiningConvexPolygons(const QVector<QPointF> &poly1, const QVector<QPointF> &poly2) {
    QVector<QPointF> allPoints = poly1 + poly2;
    return jarvisConvexHull(allPoints);
    //Это не сработает, но когда-нибудь это будет дописано
}

Path qPolygonFToPath(const QPolygonF &polygon) {
    Path path;
    for (const QPointF &point : polygon) {
        IntPoint intPoint(static_cast<cInt>(point.x() * 1000), static_cast<cInt>(point.y() * 1000));
        path.push_back(intPoint);
    }
    return path;
}

QVector<QPolygonF> pathsToQPolygons(const Paths &paths) {
    QVector<QPolygonF> polygons;
    for (const Path &path : paths) {
        QPolygonF polygon;
        for (const IntPoint &point : path) {
            QPointF qPoint(static_cast<double>(point.X) / 1000.0, static_cast<double>(point.Y) / 1000.0);
            polygon << qPoint;
        }
        polygons.append(polygon);
    }
    return polygons;
}


QVector<QPolygonF> computeIntersection(const QPolygonF &poly1, const QPolygonF &poly2) {
    Clipper clipper;
    clipper.AddPath(qPolygonFToPath(poly1), ptSubject, true);
    clipper.AddPath(qPolygonFToPath(poly2), ptClip, true);
    Paths solution;
    clipper.Execute(ctIntersection, solution, pftNonZero, pftNonZero);
    return pathsToQPolygons(solution);
}

QVector<QPolygonF> computeUnion(const QPolygonF &poly1, const QPolygonF &poly2) {
    Clipper clipper;
    clipper.AddPath(qPolygonFToPath(poly1), ptSubject, true);
    clipper.AddPath(qPolygonFToPath(poly2), ptClip, true);
    Paths solution;
    clipper.Execute(ctUnion, solution, pftNonZero, pftNonZero);
    return pathsToQPolygons(solution);
}

QVector<QPolygonF> computeDifference(const QPolygonF &poly1, const QPolygonF &poly2) {
    Clipper clipper;
    clipper.AddPath(qPolygonFToPath(poly1), ptSubject, true);
    clipper.AddPath(qPolygonFToPath(poly2), ptClip, true);
    Paths solution;
    clipper.Execute(ctDifference, solution, pftNonZero, pftNonZero);
    return pathsToQPolygons(solution);
}









