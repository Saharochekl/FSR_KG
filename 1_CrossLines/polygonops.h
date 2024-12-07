#ifndef POLYGONOPS_H
#define POLYGONOPS_H

#include "geometryutils.h"

 bool in_figure(QVector<Edge> seg, QPointF A); //перенесено
 bool point_not_in_vec(QVector<QPointF> p, QPointF A); //гот
 bool seg_in_figure(QVector<Edge> seg, Edge s);//гот

 int pred(Edge s, QPointF c); // Предикат для пересечений, по факту костыль(гот)
 int per(Edge seg1, Edge seg2); //Пересечение отрезков...(гот)
 int per_polygon(QVector <Edge> seg, QPointF A, QPointF B); //гот

 QPointF per_point(Edge seg1, Edge seg2); //гот

 QVector <Edge> do_polygon(QVector <Edge> seg1); //гот
 QVector <Edge> do_intersection(QVector <Edge> segA, QVector <Edge> segB);//гот
 QVector <Edge> do_union(QVector <Edge> segA, QVector <Edge> segB);//гот
 QVector <Edge> do_difference(QVector <Edge> segA, QVector <Edge> segB);//гот
 QVector <Edge> do_differenceSem(QVector <Edge> segA, QVector <Edge> segB);//гот



#endif // POLYGONOPS_H
