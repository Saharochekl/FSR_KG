#include "triangulation.h"

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

