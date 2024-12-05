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

int pred(Edge s, QPointF c){
    double pred = (s.p2.x() - s.p1.x())*(c.y() - s.p1.y()) - (c.x() - s.p1.x())*(s.p2.y() - s.p1.y());
    if (pred < 0)
        return -1; // Point is to the left of the segment
    else if(pred > 0)
        return 1; // Point is to the right of the segment
    else
        return 0; // Point is on the segment
}

int per(Edge seg1, Edge seg2){
    double k1 = (seg1.p2.y() - seg1.p1.y())/(seg1.p2.x() - seg1.p1.x());
    double k2 = (seg2.p2.y() - seg2.p1.y())/(seg2.p2.x() - seg2.p1.x());
    double b1 = seg1.p1.y() - k1*seg1.p1.x();
    double b2 = seg2.p1.y() - k2*seg2.p1.x();
    if (abs(k1-k2) < 0.0000001)
        return 0;
    QPoint x;
    x.setX((b2 - b1)/(k1-k2));
    x.setY(k1 * x.x() + b1);
    //cerr<<"x: "<<x<<endl;
    if ((x.x() < fmin(seg1.p1.x(), seg1.p2.x())) || (x.x() < fmin(seg2.p1.x(), seg2.p2.x())) || (x.x() > fmax(seg1.p1.x(), seg1.p2.x())) || (x.x() > fmax(seg2.p1.x(), seg2.p2.x())))
        return 0;
    if ( (dist(x, seg1.p1) < 0.0000001) || (dist(x, seg2.p1) < 0.0000001) || (dist(x, seg1.p2) < 0.0000001) || (dist(x, seg2.p2) < 0.0000001))
        return 0;
    //cerr<<"x провереный: "<<x<<endl;
    return 1;
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

// Function to calculate the angle ABC in radians
double angle(QPointF A, QPointF B, QPointF C){
    if ((C == A) || (C == B))
        return 0; // No angle if points are overlapping
    A = A - B; // Vector from B to A
    C = C - B; // Vector from B to C
    // Dot product divided by product of magnitudes gives cosine of the angle
    double cosa = (A.x() * C.x() + A.y() * C.y())/(sqrt(pow(A.x(), 2) + pow(A.y(), 2))*sqrt(pow(C.x(), 2) + pow(C.y(), 2)));
    return abs(acos(cosa)); // Return absolute value to ensure angle is positive
}

bool check(Edge s0, QVector <Edge> s){
    bool flag = true; // Assume the segment is not in the list
    Edge s1; // Create reverse segment for comparison
    s1.p1 = s0.p2;
    s1.p2 = s0.p1;
    for(int i = 0; i < s.size(); i++)
        if (s0 == s[i] || s1 == s[i]) // If either segment or its reverse is found
            flag = false; // Segment is in the list
    return flag; // Return true if segment is not in the list, false otherwise
}


QPointF nextpoint(QPointF A, QPointF B, int N, QVector <QPointF> mas){
    QPointF P0 = mas[0]; // Initial point for comparison
    double anglemax = angle(A, B, mas[0]); // Initial maximum angle
    for(int i = 1; i < N; i++){
        if (angle(A, B, mas[i]) > anglemax){ // If a larger angle is found
            anglemax = angle(A, B, mas[i]);
            P0 = mas[i]; // Update the point and maximum angle
        }else if((angle(A, B, mas[i]) == anglemax) && (dist(B, mas[i]) < dist(B, P0))){
            // If angle is the same but distance is shorter, update the point
            anglemax = angle(A, B, mas[i]);
            P0 = mas[i];
        }
    }
    return P0; // Return the point that forms the largest angle
}

QVector <Triangle> do_triang(QVector <QPointF> mas){
    int N = mas.size(); // Number of points
    QPointF a = mas[0]; // Start with the first point
    // Find the point with the smallest x (and y if x is the same)
    for(int i = 1; i < N; i++){
        if (mas[i].x() < a.x() || (mas[i].x() == a.x() && mas[i].y() < a.y()))
            a = mas[i];
    }
    QPointF a0; // Create a new point slightly above 'a' to start triangulation

    a0.setX(a.x());//Чёрная магия! работает – не трогать!!!!
    a0.setY(a.y() + 10); //Чёрная магия! работает – не трогать!!!!

    QPointF b = nextpoint(a0, a, N, mas); // Find next point to form a base segment

    QVector <Edge> seg; // Vector to store segments of the triangulation
    Edge s0; // Initial segment
    s0.p1 = a;
    s0.p2 = b;
    seg.push_back(s0); // Add the initial segment to the vector

    QVector <Edge> edges; // Vector to store edges being processed
    s0.pred = -1; // Initial direction for triangulation
    edges.push_back(s0);

    QVector <Triangle> t; // Vector to store the resulting triangles
    Triangle t0; // Initial triangle

    // Loop through edges to form triangles
    while(edges.size() > 0){
        double minangel = -1; // Minimum angle for comparison
        for (int i = 0; i < N; i++){
            // If point forms a valid angle with the edge being processed
            if((pred(edges[0], mas[i]) == -edges[0].pred) && (angle(edges[0].p1, mas[i], edges[0].p2) > minangel)){
                minangel = angle(edges[0].p1, mas[i], edges[0].p2); // Update minimum angle
                a = mas[i]; // Update point 'a' to the current point
            }
        }
        // If a valid angle was found, form new edges and triangle
        if (minangel != -1){
            // Create two new segments from the edges of the triangle
            s0.p1 = edges[0].p1;
            s0.p2 = a;
            s0.pred = pred(s0, edges[0].p2);
            if (check(s0, edges) && check(s0, seg)){ // If new segments are valid
                edges.push_back(s0); // Add to edges being processed
                seg.push_back(s0); // Add to segments of the triangulation
            }
            s0.p1 = edges[0].p2;
            s0.p2 = a;
            s0.pred = pred(s0, edges[0].p1);
            if (check(s0, edges) && check(s0, seg)){
                edges.push_back(s0);
                seg.push_back(s0);
            }

            // Form a new triangle and add it to the vector
            t0.p1 = edges[0].p1;
            t0.p2 = edges[0].p2;
            t0.p3 = a;
            t.push_back(t0);
        }
        edges.erase(edges.begin()); // Remove the processed edge
    }
    return t; // Return the vector of triangles
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




QPointF per_point(Edge seg1, Edge seg2){
    double k1 = (seg1.p2.y() - seg1.p1.y())/(seg1.p2.x() - seg1.p1.x());
    double k2 = (seg2.p2.y() - seg2.p1.y())/(seg2.p2.x() - seg2.p1.x());
    double b1 = seg1.p1.y() - k1*seg1.p1.x();
    double b2 = seg2.p1.y() - k2*seg2.p1.x();
    if (abs(k1-k2) < 0.0000001)
        return QPointF(0, 0);
    QPointF p;
    p.setX((b2 - b1)/(k1-k2));
    p.setY(k1 * p.x() + b1);
    return p;
}

int per_polygon(QVector <Edge> seg, QPointF A, QPointF B){
    int n = 0;
    Edge s;
    s.p1 = A;
    s.p2 = B;
    for (int i = 0; i < seg.size(); i++)
        n = n + per(seg[i], s);
    return n;
}

bool in_figure(QVector <Edge> seg, QPointF A){ //если луч пересечет многоугольник нечетное число раз, то лежит в многоугольнике
    bool flag = false;
    QPointF B(0, 0);
    //cerr<<"Пересечения: "<<per_polygon(seg, A, B)<<endl;
    //cerr<<A.x<<" "<<A.y<<endl;
    //cerr<<B.x<<" "<<B.y<<endl;
    if (per_polygon(seg, A, B) % 2 == 1)
        flag = true;
    return flag;
}

bool seg_in_figure(QVector <Edge> seg, Edge s){
    QPointF c;
    c = (s.p1 + s.p2)/2;
    return in_figure(seg, c);
}

bool point_not_in_vec(QVector <QPointF> p, QPointF A){
    bool f = true;
    for (int i = 0; i < p.size(); i++)
        if (dist(A, p[i]) < 0.0000000000001)
            f = false;
    return f;
}

QVector <Edge> do_polygon(QVector <Edge> seg1){
    QVector <Edge> seg;
    QPointF a0;
    QPointF a;
    a = seg1[0].p2;
    a0 = seg1[0].p1;
    seg.push_back(seg1[0]);
    seg1.erase(seg1.begin());

    while (seg1.size() > 0){
        while(dist(a0, a) > 0.000000001){
            int k = 0;
            while ((k < seg1.size()) && (dist(seg1[k].p1, a) > 0.000000001) && (dist(seg1[k].p2, a) > 0.000000001)) k++;

            if ((k < seg1.size()) && dist(seg1[k].p1, a) <= 0.000000001){
                seg.push_back(seg1[k]);
                a = seg1[k].p2;
                seg1.erase(seg1.begin() + k);
            }

            if ((k < seg1.size()) && dist(seg1[k].p2, a) <= 0.000000001){
                seg.push_back(Edge(seg1[k].p2, seg1[k].p1));
                a = seg1[k].p1;
                seg1.erase(seg1.begin() + k);
            }
        }
        if (seg1.size() > 0){
            a = seg1[0].p2;
            a0 = seg1[0].p1;
            seg.push_back(seg1[0]);
            seg1.erase(seg1.begin());
        }
    }
    return seg;
}

//Триангуляция многоугольника - Работает, не тогатть
QVector <Triangle> do_triang_poligon(QVector <Edge> seg){
    QVector <Triangle> t;
    QVector <QPointF> p;
    QPointF c;
    for (int i = 0; i < seg.size(); i++)
        p.push_back(seg[i].p1);
    //добавляем точки само-пересечения
    for (int i = 0; i < seg.size() - 1; i++)
        for (int j = i + 1; j < seg.size(); j++)
            if (per(seg[i], seg[j]) == 1)
                p.push_back(per_point(seg[i] , seg[j]));
    t = do_triang(p);
    //cerr<< t.size()<<endl;
    int i = 0;
    while (i < t.size()){
        c = (t[i].p1 + t[i].p2)/2;
        c = (c + t[i].p3)/2;
        //cerr<<"("<<c.x<<", "<<c.y<<")"<<endl;
        if (!in_figure(seg, c))
            t.erase(t.begin() + i);
        else
            i++;
        //cerr<<in_figure(seg, c)<<endl;
    }
    return t;
}
//Триангуляция многоугольника - Работает, не тогатть

QVector <Triangle> do_intersection_trisngl(QVector <Edge> segA, QVector <Edge> segB){
    QVector <Triangle> t;
    QVector <QPointF> p;
    QPointF c;
    for (int i = 0; i < segA.size(); i++)
        p.push_back(segA[i].p1);
    for (int i = 0; i < segB.size(); i++)
        p.push_back(segB[i].p1);

    for (int i = 0; i < segA.size(); i++)
        for (int j = 0; j < segB.size(); j++)
            if (per(segA[i], segB[j]) == 1)
                p.push_back(per_point(segA[i] , segB[j]));

    t = do_triang(p);
    //cerr<< t.size()<<endl;
    int i = 0;
    while (i < t.size()){
        c = (t[i].p1 + t[i].p2)/2;
        c = (c + t[i].p3)/2;
        //cerr<<"("<<c.x<<", "<<c.y<<")"<<endl;
        if (!in_figure(segA, c) || !in_figure(segB, c))
            t.erase(t.begin() + i);
        else
            i++;
        //cerr<<in_figure(seg, c)<<endl;
    }
    return t;
}

QVector <Edge> do_intersection(QVector <Edge> segA, QVector <Edge> segB){
    //vector <triangl> tA = do_triang_poligon(segA);
    //vector <triangl> tB = do_triang_poligon(segB);
    //в segA делим все отрезки в точках пересечения
    QVector <Edge> segAA;
    for (int i = 0; i < segA.size(); i++){
        QVector <QPointF> pper;
        pper.push_back(segA[i].p2);
        for (int j = 0; j < segB.size(); j++)
            if (per(segA[i], segB[j]) == 1)
                pper.push_back(per_point(segA[i] , segB[j]));
        QPointF a = segA[i].p1;
        while (pper.size() > 0){
            double min_dist = dist(a, pper[0]);
            int k = 0;
            for (int j = 1; j < pper.size(); j++)
                if (dist(a, pper[j]) < min_dist){
                    k = j;
                    min_dist = dist(a, pper[j]);
                }
            segAA.push_back(Edge(a, pper[k]));
            a = pper[k];
            pper.erase(pper.begin() + k);
        }
    }

    //в segB делим все отрезки в точках пересечения
    QVector <Edge> segBB;
    for (int i = 0; i < segB.size(); i++){
        QVector <QPointF> pper;
        pper.push_back(segB[i].p2);
        for (int j = 0; j < segA.size(); j++)
            if (per(segB[i], segA[j]) == 1)
                pper.push_back(per_point(segB[i] , segA[j]));
        QPointF a = segB[i].p1;
        while (pper.size() > 0){
            double min_dist = dist(a, pper[0]);
            int k = 0;
            for (int j = 1; j < pper.size(); j++)
                if (dist(a, pper[j]) < min_dist){
                    k = j;
                    min_dist = dist(a, pper[j]);
                }
            segBB.push_back(Edge(a, pper[k]));
            a = pper[k];
            pper.erase(pper.begin() + k);
        }
    }

    QVector <Edge> seg;

    for (int i = 0; i < segAA.size(); i++){
        if (seg_in_figure(segB, segAA[i]))
            seg.push_back(segAA[i]);
    }

    for (int i = 0; i < segBB.size(); i++){
        if (seg_in_figure(segA, segBB[i]))
            seg.push_back(segBB[i]);
    }

    seg = do_polygon(seg);

    return seg;
}

QVector <Edge> do_union(QVector <Edge> segA, QVector <Edge> segB){
    QVector <Edge> segAA;
    for (int i = 0; i < segA.size(); i++){
        QVector <QPointF> pper;
        pper.push_back(segA[i].p2);
        for (int j = 0; j < segB.size(); j++)
            if (per(segA[i], segB[j]) == 1)
                pper.push_back(per_point(segA[i] , segB[j]));
        QPointF a = segA[i].p1;
        while (pper.size() > 0){
            double min_dist = dist(a, pper[0]);
            int k = 0;
            for (int j = 1; j < pper.size(); j++)
                if (dist(a, pper[j]) < min_dist){
                    k = j;
                    min_dist = dist(a, pper[j]);
                }
            segAA.push_back(Edge(a, pper[k]));
            a = pper[k];
            pper.erase(pper.begin() + k);
        }
    }

    //в segB делим все отрезки в точках пересечения
    QVector <Edge>  segBB;
    for (int i = 0; i < segB.size(); i++){
        QVector <QPointF> pper;
        pper.push_back(segB[i].p2);
        for (int j = 0; j < segA.size(); j++)
            if (per(segB[i], segA[j]) == 1)
                pper.push_back(per_point(segB[i] , segA[j]));
        QPointF a = segB[i].p1;
        while (pper.size() > 0){
            double min_dist = dist(a, pper[0]);
            int k = 0;
            for (int j = 1; j < pper.size(); j++)
                if (dist(a, pper[j]) < min_dist){
                    k = j;
                    min_dist = dist(a, pper[j]);
                }
            segBB.push_back(Edge(a, pper[k]));
            a = pper[k];
            pper.erase(pper.begin() + k);
        }
    }

    QVector <Edge> seg;

    for (int i = 0; i < segAA.size(); i++){
        if (!seg_in_figure(segB, segAA[i]))
            seg.push_back(segAA[i]);
    }

    for (int i = 0; i < segBB.size(); i++){
        if (!seg_in_figure(segA, segBB[i]))
            seg.push_back(segBB[i]);
    }

    seg = do_polygon(seg);

    return seg;
}

//A без B
QVector <Edge> do_difference(QVector <Edge> segA, QVector <Edge> segB){
    QVector <Edge> segAA;
    for (int i = 0; i < segA.size(); i++){
        QVector <QPointF> pper;
        pper.push_back(segA[i].p2);
        for (int j = 0; j < segB.size(); j++)
            if (per(segA[i], segB[j]) == 1)
                pper.push_back(per_point(segA[i] , segB[j]));
        QPointF a = segA[i].p1;
        while (pper.size() > 0){
            double min_dist = dist(a, pper[0]);
            int k = 0;
            for (int j = 1; j < pper.size(); j++)
                if (dist(a, pper[j]) < min_dist){
                    k = j;
                    min_dist = dist(a, pper[j]);
                }
            segAA.push_back(Edge(a, pper[k]));
            a = pper[k];
            pper.erase(pper.begin() + k);
        }
    }

    //в segB делим все отрезки в точках пересечения
    QVector <Edge> segBB;
    for (int i = 0; i < segB.size(); i++){
        QVector <QPointF> pper;
        pper.push_back(segB[i].p2);
        for (int j = 0; j < segA.size(); j++)
            if (per(segB[i], segA[j]) == 1)
                pper.push_back(per_point(segB[i] , segA[j]));
        QPointF a = segB[i].p1;
        while (pper.size() > 0){
            double min_dist = dist(a, pper[0]);
            int k = 0;
            for (int j = 1; j < pper.size(); j++)
                if (dist(a, pper[j]) < min_dist){
                    k = j;
                    min_dist = dist(a, pper[j]);
                }
            segBB.push_back(Edge(a, pper[k]));
            a = pper[k];
            pper.erase(pper.begin() + k);
        }
    }

    QVector <Edge> seg;

    for (int i = 0; i < segAA.size(); i++){
        if (!seg_in_figure(segB, segAA[i]))
            seg.push_back(segAA[i]);
    }

    for (int i = 0; i < segBB.size(); i++){
        if (seg_in_figure(segA, segBB[i]))
            seg.push_back(segBB[i]);
    }

    seg = do_polygon(seg);

    return seg;
}

//Семетрическая разность
QVector <Edge> do_differenceSem(QVector <Edge> segA, QVector <Edge> segB){
    QVector <Edge> segAA;
    for (int i = 0; i < segA.size(); i++){
        QVector <QPointF> pper;
        pper.push_back(segA[i].p2);
        for (int j = 0; j < segB.size(); j++)
            if (per(segA[i], segB[j]) == 1)
                pper.push_back(per_point(segA[i] , segB[j]));
        QPointF a = segA[i].p1;
        while (pper.size() > 0){
            double min_dist = dist(a, pper[0]);
            int k = 0;
            for (int j = 1; j < pper.size(); j++)
                if (dist(a, pper[j]) < min_dist){
                    k = j;
                    min_dist = dist(a, pper[j]);
                }
            segAA.push_back(Edge(a, pper[k]));
            a = pper[k];
            pper.erase(pper.begin() + k);
        }
    }

    //в segB делим все отрезки в точках пересечения
    QVector <Edge> segBB;
    for (int i = 0; i < segB.size(); i++){
        QVector <QPointF> pper;
        pper.push_back(segB[i].p2);
        for (int j = 0; j < segA.size(); j++)
            if (per(segB[i], segA[j]) == 1)
                pper.push_back(per_point(segB[i] , segA[j]));
        QPointF a = segB[i].p1;
        while (pper.size() > 0){
            double min_dist = dist(a, pper[0]);
            int k = 0;
            for (int j = 1; j < pper.size(); j++)
                if (dist(a, pper[j]) < min_dist){
                    k = j;
                    min_dist = dist(a, pper[j]);
                }
            segBB.push_back(Edge(a, pper[k]));
            a = pper[k];
            pper.erase(pper.begin() + k);
        }
    }

    QVector <Edge> seg;

    for (int i = 0; i < segAA.size(); i++){
        if (!seg_in_figure(segB, segAA[i]))
            seg.push_back(segAA[i]);
    }

    for (int i = 0; i < segBB.size(); i++){
        if (seg_in_figure(segA, segBB[i]))
            seg.push_back(segBB[i]);
    }

    for (int i = 0; i < segAA.size(); i++){
        if (seg_in_figure(segB, segAA[i]))
            seg.push_back(segAA[i]);
    }

    for (int i = 0; i < segBB.size(); i++){
        if (!seg_in_figure(segA, segBB[i]))
            seg.push_back(segBB[i]);
    }

    seg = do_polygon(seg);

    return seg;
}

