#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H
#include <QPointF>
#include <math.h>
#include <QPoint>

int orientation(QPoint p, QPoint q, QPoint r);
bool onSegment(QPoint p, QPoint q, QPoint r);
bool doIntersect(QPoint A, QPoint B, QPoint C, QPoint D);


double vectorProduct(QPointF p1, QPointF p2);
double distance(QPointF p1, QPointF p2);
double norma(QPointF vec);
double angleBetwenDeg(QPointF p1, QPointF p2, QPointF d1, QPointF d2);
int orientation(QPoint p, QPoint q, QPoint r)

#endif // MATHFUNCTIONS_H
