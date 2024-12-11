
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
        QVector<QPointF> LinearCurve = generateLinearBezier(vecPoint);
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
        QVector<QPointF> QuadraticCurve = generateQuadraticBezier(vecPoint, 100);
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
        QVector<QPointF> CubicCurve = generateCubicBezier(vecPoint);
        painter.setPen(QPen("orange"));
        for (int i = 0; i < CubicCurve.size() - 1; ++i) {
            painter.drawLine(CubicCurve[i], CubicCurve[i + 1]);
        }
    }
    if(isManualChecked)
    {
        QVector<QPointF> ManualCurve = generateManualBezier(vecPoint, degree);
        painter.setPen(QPen(Qt::green, 2));
        for (int i = 0; i < ManualCurve.size() - 1; ++i) {
            painter.drawLine(ManualCurve[i], ManualCurve[i + 1]);
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
//        if (vecPoint.size() >= 4) {
//            vecPoint.clear();
//        }
        vecPoint.push_back(newPoint);

        // Эмитируем сигнал для передачи новой точки
        emit dataChanged(newPoint);
    }
    else if (pe->button() == Qt::RightButton)
    {
        layMove = true;

//        if (typeTask == task1 || typeTask == task2 || typeTask == task3)
//        {
//            if (!vecPoint.isEmpty())
//            {
//                double radius = 10;
//                double closestDistance = std::numeric_limits<double>::max();
//                int closestIndex = -1;

//                for (int i = 0; i < vecPoint.size(); ++i)
//                {
//                    double distanceToCurrent = dist(vecPoint[i], pe->localPos());
//                    if (distanceToCurrent < radius && distanceToCurrent < closestDistance)
//                    {
//                        closestDistance = distanceToCurrent;
//                        closestIndex = i;
//                    }
//                }
//                m_id = closestIndex;
//            }
//        }
    }

    update();  // Обновление виджета для отображения новых изменений
}


//void CustomDrawWidget::mouseDoubleClickEvent(QMouseEvent * pe)
//{
//    if (pe->button() == Qt::RightButton)
//    {
//        if (typeTask == task5)
//        {
//            isFirstPolygon = false;
//        }
//    }
//}


//void CustomDrawWidget::mouseMoveEvent(QMouseEvent * pe)
//{
//    if (layMove == true)
//    {
//        if (typeTask == task1 or typeTask == task2 or typeTask == task3)
//            if (m_id != -1)
//            {
//                vecPoint[m_id] = pe->position();
//                repaint();
//            }
//        if (typeTask == task5)
//        {

//        }
//    }
//}
void CustomDrawWidget::mouseReleaseEvent(QMouseEvent * pe)
{
    if (pe->button() == Qt::RightButton)
    {
        layMove = false;
    }
}
//void CustomDrawWidget::wheelEvent(QMouseEvent * pe)
//{

//}

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
//    if(typeTask == task3){
//        hullPoints.clear();
//    }
//    if(typeTask == task4){
//        triangulationEdges.clear();
//    }
//    if (typeTask == task5)
//    {
//        isFirstPolygon = true;
//        Polygon1.clear();
//        Polygon2.clear();
//        resultPolygons.clear();
//    }
//    if (typeTask == task6)
//    {
//        isFirstPolygon = true;
//        Polygon1.clear();
//    }
    repaint();
}


