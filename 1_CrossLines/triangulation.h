#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include "geometryutils.h" // для базовых функций
#include "polygonops.h"

 QVector<Triangle> delaunayTriangulation(const QVector<QPointF>& points);
 QVector<Triangle> do_triang(QVector<QPointF> mas);
 QVector<Triangle> do_triang_poligon(QVector<Edge> seg);
 QVector<Triangle> do_intersection_trisngl(QVector<Edge> segA, QVector<Edge> segB);

 QVector<Edge> getTriangulation(const QVector<QPointF> &points, QStringList &logMessages);


#endif // TRIANGULATION_H
