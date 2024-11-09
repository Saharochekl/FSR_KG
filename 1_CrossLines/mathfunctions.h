#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H
#include <QPointF>
#include <math.h>
#include <QPoint>

double vectorProduct(QPointF p1, QPointF p2);
double distance(QPointF p1, QPointF p2);
double norma(QPointF vec);
double angleBetwenDeg(QPointF p1, QPointF p2, QPointF d1, QPointF d2);
int orientation(QPointF p, QPointF q, QPointF r);
bool onSegment(QPointF p, QPointF q, QPointF r);
bool doIntersect(QPointF A, QPointF B, QPointF C, QPointF D, QPointF& intersection);

#endif // MATHFUNCTIONS_H
