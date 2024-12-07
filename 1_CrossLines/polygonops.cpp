#include "polygonops.h"


bool in_figure(QVector <Edge> seg, QPointF A){ //если луч пересечет многоугольник нечетное число раз, то лежит в многоугольнике
    bool flag = false;
    QPointF B(0, 0);

    if (per_polygon(seg, A, B) % 2 == 1)
        flag = true;
    return flag;
}

bool point_not_in_vec(QVector <QPointF> p, QPointF A){
    bool f = true;
    for (int i = 0; i < p.size(); i++)
        if (dist(A, p[i]) < 0.0000000000001)
            f = false;
    return f;
}

bool seg_in_figure(QVector <Edge> seg, Edge s){
    QPointF c;
    c = (s.p1 + s.p2)/2;
    return in_figure(seg, c);
}

/*
int pred(Edge s, QPointF c){
    double pred = (s.p2.x() - s.p1.x())*(c.y() - s.p1.y()) - (c.x() - s.p1.x())*(s.p2.y() - s.p1.y());
    if (pred < 0)
        return -1; // Point is to the left of the segment
    else if(pred > 0)
        return 1; // Point is to the right of the segment
    else
        return 0; // Point is on the segment
}
*/

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

int per_polygon(QVector <Edge> seg, QPointF A, QPointF B){
    int n = 0;
    Edge s;
    s.p1 = A;
    s.p2 = B;
    for (int i = 0; i < seg.size(); i++)
        n = n + per(seg[i], s);
    return n;
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

