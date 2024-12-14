#ifndef BEZIERUTILS_H
#define BEZIERUTILS_H
#include <QPointF>
#include <math.h>
#include <cmath>
#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include <cassert>

#include <QVector>
#include <QSet>
#include <algorithm>

// Структура, представляющая один терм полинома вида coef * (1 - t)^a * t^b
struct Term {
    double coef;
    int a;
    int b;
};

struct Condition {
    int derivativeOrder;
    double tValue;
    double knownValueX;
    double knownValueY;

};

struct Edge {
    QPointF p1, p2;
    int pred; // для пересечения

    Edge() : pred(0) {}
    Edge(const QPointF& a, const QPointF& b) : p1(a), p2(b), pred(0) {}

    bool operator==(const Edge& other) const {
        return ((p1 == other.p1 && p2 == other.p2) ||
                (p1 == other.p2 && p2 == other.p1));
    }
};


std::vector<double> bezierCoefficients(int d, double t);
std::vector<Term> combineLikeTerms(const std::vector<Term>& terms);
std::vector<Term> differentiateTerms(const std::vector<Term>& terms);
std::vector<Term> bezierBaseTerms(int d);
double evaluateTerms(const std::vector<Term>& terms, double t);
std::pair<std::vector<std::vector<double>>, std::vector<double>>
buildSystemForBezierWithDerivatives(int d, const std::vector<Condition>& conditions);

std::vector<double> getBezierCoefficientsForDerivative(int d, int derivativeOrder, double t);
std::vector<double> Gauss_SLAY(std::vector<std::vector<double>> A, std::vector<double> b);


QVector<QPointF> generateBezierChainFromPoints(const QVector<QPointF>& inputPoints, int d, int segmentsPerCurve);


QVector<QPointF> computeQuadraticBezierChain(const QVector<QPointF>& points, int segmentsPerCurve);

QVector<QPointF> generateSegmentedBezier(const QVector<QPointF>& points, int segments, int degree);


QVector<QPointF> generateManualBezier(const QVector<QPointF>& points, int segments);


#endif // BEZIERUTILS_H
