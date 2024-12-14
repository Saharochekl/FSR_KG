
#include "bezierutils.h"
#include "customdrawwidget.h"
#include <QPainter>
#include <iostream>

CustomDrawWidget::CustomDrawWidget(QWidget * parent) : QWidget(parent)
{
    m_color = Qt::red;

    scale = 10;

}

double CustomDrawWidget::normalizeValue(const LongDouble value, const LongDouble maxValue, double maxRange)
{
    return ((value / maxValue).toDouble() * maxRange);
}

void CustomDrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::red, 2);
    painter.setPen(pen);

    // Рисуем изначальную ломаную кривую
    for (int i = 0; i < vecPoint.size() - 1; ++i) {
        painter.drawLine(vecPoint[i], vecPoint[i + 1]);
    }

    // Отображение контрольных точек
    QBrush pointBrush(Qt::green);
    for (const QPointF& point : vecPoint) {
        painter.setBrush(pointBrush);
        painter.drawEllipse(point, 3, 3);
    }

    if (isLinearChecked) {
        QVector<QPointF> LinearCurve = generateBezierChainFromPoints(vecPoint, 1, 100);
        QPen pen2;
        pen2.setColor(Qt::blue);
        pen2.setWidth(1.5);
        painter.setPen(pen2);
        for (int i = 0; i < LinearCurve.size() - 1; ++i) {
            painter.drawLine(LinearCurve[i], LinearCurve[i + 1]);
        }
    }
    if(isQuadraticChecked)
    {
        QVector<QPointF> QuadraticCurve = generateBezierChainFromPoints(vecPoint, 2, 100);
        QPen pen2;
        pen2.setColor(Qt::green);
        pen2.setWidth(1.5);
        painter.setPen(pen2);
        for (int i = 0; i < QuadraticCurve.size() - 1; ++i) {
            painter.drawLine(QuadraticCurve[i], QuadraticCurve[i + 1]);
        }
    }
    if(isCubicChecked)
    {
        QVector<QPointF> CubicCurve = generateBezierChainFromPoints(vecPoint, 3, 100);
        painter.setPen(QPen("orange"));
        for (int i = 0; i < CubicCurve.size() - 1; ++i) {
            painter.drawLine(CubicCurve[i], CubicCurve[i + 1]);
        }
    }
    if(isManualChecked)
    {
        QVector<QPointF> ManualCurve = generateBezierChainFromPoints(vecPoint, degree, 100);
        painter.setPen(QPen(Qt::white, 2));
        for (int i = 0; i < ManualCurve.size() - 1; ++i) {
            painter.drawLine(ManualCurve[i], ManualCurve[i + 1]);
        }
    }

    if(isnumP_curveChecked){
        QVector<QPointF> NumPCurve = generateManualBezier(vecPoint, 100);
        painter.setPen(QPen(Qt::yellow, 2));
        for (int i = 0; i < NumPCurve.size() - 1; ++i) {
            painter.drawLine(NumPCurve[i], NumPCurve[i + 1]);
        }
    }
}

void CustomDrawWidget::changedColor(QColor color)
{
    m_color = color;
    repaint();
}


void CustomDrawWidget::mousePressEvent(QMouseEvent *pe)
{
    if (pe->button() == Qt::LeftButton)
    {

        QPointF newPoint = pe->position();

        vecPoint.push_back(newPoint);

        // Эмитируем сигнал для передачи новой точки
        emit dataChanged(newPoint);
    }
    else if (pe->button() == Qt::RightButton)
    {
        layMove = true;


    }

    update();  // Обновление виджета для отображения новых изменений
}



void CustomDrawWidget::mouseReleaseEvent(QMouseEvent * pe)
{
    if (pe->button() == Qt::RightButton)
    {
        layMove = false;
    }
}

void CustomDrawWidget::setDegree(int d){
    if (d > 0) {
        degree = d;
        update(); // Перерисовать виджет
    }
}

void CustomDrawWidget::clearVector()
{
    vecPoint.clear();
    Polygon1.clear();
    resultPolygons.clear();

    repaint();
}


