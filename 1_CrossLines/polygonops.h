#ifndef POLYGONOPS_H
#define POLYGONOPS_H

#include "geometryutils.h"

 bool in_figure(QVector<Edge> seg, QPointF A);
 bool point_not_in_vec(QVector<QPointF> p, QPointF A);
 bool seg_in_figure(QVector<Edge> seg, Edge s);

 int per(Edge seg1, Edge seg2); //Пересечение отрезков...
 int per_polygon(QVector <Edge> seg, QPointF A, QPointF B);

 QPointF per_point(Edge seg1, Edge seg2);

 QVector <Edge> do_polygon(QVector <Edge> seg1);
 QVector <Edge> do_intersection(QVector <Edge> segA, QVector <Edge> segB);
 QVector <Edge> do_union(QVector <Edge> segA, QVector <Edge> segB);
 QVector <Edge> do_difference(QVector <Edge> segA, QVector <Edge> segB);



#endif // POLYGONOPS_H
