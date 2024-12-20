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
// Генерация базовых термов для кривой Безье степени d (без учёта контрольных точек B_k).
// Кривая Безье: B(t) = sum_{k=0}^{d} B_k * C(d,k)*(1-t)^{d-k} t^k
// Здесь мы генерируем лишь C(d,k)*(1-t)^{d-k}*t^k, без умножения на B_k.
std::vector<Term> bezierBaseTerms(int d) {
    std::vector<Term> terms;
    for (int k = 0; k <= d; ++k) {
        double c = binomial_coefficient(d, k);
        Term T{c, d-k, k};
        terms.push_back(T);
    }
    return terms;
}

// Функция для вычисления значения набора термов в точке t
double evaluateTerms(const std::vector<Term>& terms, double t) {
    double val = 0.0;
    for (auto &term : terms) {
        val += term.coef * std::pow(1.0 - t, term.a) * std::pow(t, term.b);
    }
    return val;
}

// Функция для объединения подобных термов, т.к. при дифференцировании может получиться несколько термов с одинаковыми (a,b)
std::vector<Term> combineLikeTerms(const std::vector<Term>& terms) {
    std::map<std::pair<int,int>, double> dict;
    for (auto &tr : terms) {
        dict[{tr.a,tr.b}] += tr.coef;
    }
    std::vector<Term> res;
    for (auto &kv : dict) {
        if (std::fabs(kv.second) > 1e-14) {
            Term t{kv.second, kv.first.first, kv.first.second};
            res.push_back(t);
        }
    }
    return res;
}

// Функция дифференцирования по t:
// d/dt[(1 - t)^a t^b] = a(1-t)^{a-1}(-1)*t^b + b(1-t)^a t^{b-1}
std::vector<Term> differentiateTerms(const std::vector<Term>& terms) {
    std::vector<Term> dterms;
    for (auto &term : terms) {
        double c = term.coef;
        int a = term.a;
        int b = term.b;
        // Первая часть производной
        if (a > 0) {
            Term t1;
            t1.coef = c * a * (-1.0);
            t1.a = a - 1;
            t1.b = b;
            dterms.push_back(t1);
        }
        // Вторая часть производной
        if (b > 0) {
            Term t2;
            t2.coef = c * b;
            t2.a = a;
            t2.b = b - 1;
            dterms.push_back(t2);
        }
    }
    return combineLikeTerms(dterms);
}



std::vector<double> getBezierCoefficientsForDerivative(int d, int derivativeOrder, double t) {
    // Получим базовые термы для кривой Безье степени d
    std::vector<Term> base = bezierBaseTerms(d);
    // base[k] соответствует терму для B_k: C(d,k)*(1-t)^{d-k}*t^k

    std::vector<double> coeffs(d+1, 0.0);
    for (int k = 0; k <= d; ++k) {
        // Начинаем с терма, соответствующего B_k
        std::vector<Term> current = { base[k] };

        // Дифференцируем term `derivativeOrder` раз
        for (int r = 0; r < derivativeOrder; ++r) {
            current = differentiateTerms(current);
        }

        // Вычисляем значение в точке t
        double val = evaluateTerms(current, t);
        coeffs[k] = val;
    }

    return coeffs;
}
std::vector<double> bezierCoefficients(int d, double t) {
    std::vector<double> coeffs(d+1, 0.0);
    for (int k = 0; k <= d; ++k) {
        double c = binomial_coefficient(d, k) * std::pow(1.0 - t, d - k) * std::pow(t, k);
        coeffs[k] = c;
    }
    return coeffs;
}






std::pair<std::vector<std::vector<double>>, std::vector<double>>
buildSystemForBezierWithDerivatives(int d, const std::vector<Condition>& conditions)
{
    // Число уравнений = количество conditions
    int m = (int)conditions.size();
    if (m != d+1) {
        // Обычно для точного решения нужно m = d+1, но можно и переопределённую систему получить.
        // Для простоты потребуем равенство:
        // Если уравнений больше, чем неизвестных, будет переопределенная система.
        // Если меньше - недоопределённая. В реальном коде вы можете обрабатывать эти случаи как хотите.
        // Здесь просто примем m = d+1.
        // Если хотите, можете убрать это условие.
        std::cerr << "Warning: number of conditions != d+1. System may be unsolvable or need least squares." << std::endl;
    }

    // Инициализация матрицы A и вектора b
    std::vector<std::vector<double>> A(m, std::vector<double>(d+1, 0.0));
    std::vector<double> b(m, 0.0);

    // Заполняем систему уравнений
    for (int i = 0; i < m; ++i) {
        int order = conditions[i].derivativeOrder;
        double t = conditions[i].tValue;
        double valueX = conditions[i].knownValueX;

        // Получаем коэффициенты для этой производной
        std::vector<double> rowCoeffs = getBezierCoefficientsForDerivative(d, order, t);

        // Записываем в A и b
        for (int k = 0; k <= d; ++k) {
            A[i][k] = rowCoeffs[k];
        }
        b[i] = valueX;
    }

    return {A, b};
}



std::vector<double> Gauss_SLAY(std::vector<std::vector<double>> A, std::vector<double> b) {
    int n = A.size(); // Число уравнений

    // Прямой ход: Преобразуем матрицу A к верхнетреугольному виду
    for (int i = 0; i < n; ++i) {
        // Находим ведущий элемент
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (std::fabs(A[k][i]) > std::fabs(A[maxRow][i])) {
                maxRow = k;
            }
        }

        // Меняем строки местами (в матрице A и векторе b)
        std::swap(A[i], A[maxRow]);
        std::swap(b[i], b[maxRow]);

        // Проверяем, что матрица не вырождена
        if (std::fabs(A[i][i]) < 1e-12) {
            throw std::runtime_error("Matrix is singular or nearly singular.");
        }

        // Приводим текущую строку к каноническому виду
        for (int k = i + 1; k < n; ++k) {
            double coeff = A[k][i] / A[i][i];
            b[k] -= coeff * b[i];
            for (int j = i; j < n; ++j) {
                A[k][j] -= coeff * A[i][j];
            }
        }
    }

    // Обратный ход: находим решения x
    std::vector<double> x(n);
    for (int i = n - 1; i >= 0; --i) {
        x[i] = b[i] / A[i][i];
        for (int k = i - 1; k >= 0; --k) {
            b[k] -= A[k][i] * x[i];
        }
    }

    return x;
}


QVector<QPointF> generateBezierChainFromPoints(const QVector<QPointF>& inputPoints, int d, int segmentsPerCurve) {
    QVector<QPointF> result;
    int N = inputPoints.size();
    if (N < d+1) {
        // Недостаточно точек для построения даже одного сегмента
        return result;
    }

    // Проверим, можно ли разбить на сегменты по d
    int numSegments = (N - 1) / d;
    // Если (N-1) не делится на d, возможно, стоит обработать последний сегмент отдельно
    // Для простоты сейчас предположим, что (N-1) % d == 0.

    for (int s = 0; s < numSegments; ++s) {
        // Сегмент состоит из точек: P_{s*d}, P_{s*d+1}, ..., P_{s*d+d}
        QVector<QPointF> segmentPoints;
        for (int i = 0; i <= d; ++i) {
            segmentPoints.push_back(inputPoints[s*d + i]);
        }

        // Формируем условия интерполяции для данного сегмента:
        // B(i/d) = segmentPoints[i]
        std::vector<Condition> conditionsX;
        conditionsX.reserve(d+1);

        std::vector<Condition> conditionsY;
        conditionsY.reserve(d+1);

        for (int i = 0; i <= d; ++i) {
            double t = (double)i / d;
            // Условие для X:
            Condition cX;
            cX.derivativeOrder = 0;
            cX.tValue = t;
            cX.knownValueX = segmentPoints[i].x();
            cX.knownValueY = 0.0; // Не используется

            conditionsX.push_back(cX);

            // Условие для Y (используем knownValueX для Y):
            Condition cY;
            cY.derivativeOrder = 0;
            cY.tValue = t;
            cY.knownValueX = segmentPoints[i].y(); // Переиспользуем поле knownValueX для Y-координат
            cY.knownValueY = 0.0;
            conditionsY.push_back(cY);
        }

        // Решаем для X:
        auto [AX, bX] = buildSystemForBezierWithDerivatives(d, conditionsX);
        std::vector<double> controlPointsX = Gauss_SLAY(AX, bX);

        // Решаем для Y:
        auto [AY, bY] = buildSystemForBezierWithDerivatives(d, conditionsY);
        std::vector<double> controlPointsY = Gauss_SLAY(AY, bY);

        // Строим точки данного сегмента
        // Дискретизируем параметр t от 0 до 1
        for (int seg = 0; seg <= segmentsPerCurve; ++seg) {
            double t = (double)seg / segmentsPerCurve;
            std::vector<double> c = bezierCoefficients(d, t);
            double xVal = 0.0;
            double yVal = 0.0;
            for (int k = 0; k <= d; ++k) {
                xVal += controlPointsX[k] * c[k];
                yVal += controlPointsY[k] * c[k];
            }
            // Чтобы не дублировать точки в стыке сегментов, можно пропускать последнюю точку каждого сегмента, кроме последнего
            // Однако, если хотим точное совпадение, можно добавить все.
            // Здесь добавим все, а потом при желании можно убрать дубликаты.

            result.push_back(QPointF(xVal, yVal));
        }


    }

    return result;
}




QVector<QPointF> generateManualBezier(const QVector<QPointF>& points, int segments) {
    QVector<QPointF> result;
    int degree = points.size()-1;
//    if (points.size() < degree + 1) return result; // Проверяем, достаточно ли точек

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


