#include "bezierutils.h"
#include <cmath>

double binomial_coefficient(int n, int k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;

    double result = 1;
    for (int i = 1; i <= k; ++i) {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
}

QVector<QPointF> generateLinearBezier(const QVector<QPointF>& points, int segments) {
    QVector<QPointF> result;
    if (points.size() < 2) return result; // Линейная кривая требует минимум 2 точки

    for (int i = 0; i <= segments; ++i) {
        double t = static_cast<double>(i) / segments;
        QPointF point = (1 - t) * points[0] + t * points[1];
        result.push_back(point);
    }
    return result;
}

QVector<QPointF> generateQuadraticBezier(const QVector<QPointF>& points, int segments) {
    QVector<QPointF> result;
    if (points.size() < 3) return result; // Квадратичная кривая требует минимум 3 точки

    for (int k = 0; k < points.size() - 2; ++k) { // Перебираем сегменты
        for (int i = 0; i <= segments; ++i) {
            double t = static_cast<double>(i) / segments;
            QPointF point = (1 - t) * (1 - t) * points[k] +
                            2 * (1 - t) * t * points[k + 1] +
                            t * t * points[k + 2];
            result.push_back(point);
        }
    }
    return result;
}

QVector<QPointF> generateCubicBezier(const QVector<QPointF>& points, int segments) {
    QVector<QPointF> result;
    if (points.size() < 4) return result; // Кубическая кривая требует минимум 4 точки

    for (int i = 0; i <= segments; ++i) {
        double t = static_cast<double>(i) / segments;
        QPointF point = (1 - t) * (1 - t) * (1 - t) * points[0] +
                        3 * (1 - t) * (1 - t) * t * points[1] +
                        3 * (1 - t) * t * t * points[2] +
                        t * t * t * points[3];
        result.push_back(point);
    }
    return result;
}

QVector<QPointF> generateManualBezier(const QVector<QPointF>& points, int degree, int segments) {
    QVector<QPointF> result;
    if (points.size() < degree + 1) return result; // Проверяем, достаточно ли точек

    for (int i = 0; i <= segments; ++i) {
        double t = static_cast<double>(i) / segments;
        QPointF point(0, 0);

        for (int j = 0; j <= degree; ++j) {
            double coefficient = binomial_coefficient(degree, j) *
                                 std::pow(1 - t, degree - j) *
                                 std::pow(t, j);
            point += coefficient * points[j];
        }

        result.push_back(point);
    }
    return result;
}
