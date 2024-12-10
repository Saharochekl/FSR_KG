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
    if (typeTask == task1)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPen pen;
        pen.setColor(m_color);
        pen.setWidth(1.5);
        painter.setPen(pen);

        int size_ = vecPoint.size();

        switch (size_)
        {
        case 0:
            break;
        case 1:
            painter.drawEllipse(vecPoint[0].x()-2.5, vecPoint[0].y()-2.5, 5,5);
            break;
        case 2:
            painter.drawLine(vecPoint[0], vecPoint[1]);
            break;
        case 3:
            painter.drawLine(vecPoint[0], vecPoint[1]);
            painter.drawEllipse(vecPoint[2].x()-2.5, vecPoint[2].y()-2.5, 5,5);
            break;
        case 4:
            painter.drawLine(vecPoint[0], vecPoint[1]);
            painter.drawLine(vecPoint[2], vecPoint[3]);
            break;
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
        if (vecPoint.size() >= 4) {
            vecPoint.clear();
        }
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


void CustomDrawWidget::mouseDoubleClickEvent(QMouseEvent * pe)
{
    if (pe->button() == Qt::RightButton)
    {
        if (typeTask == task5)
        {
            isFirstPolygon = false;
        }
    }
}


void CustomDrawWidget::mouseMoveEvent(QMouseEvent * pe)
{
    if (layMove == true)
    {
        if (typeTask == task1 or typeTask == task2 or typeTask == task3)
            if (m_id != -1)
            {
                vecPoint[m_id] = pe->position();
                repaint();
            }
        if (typeTask == task5)
        {

        }
    }
}
void CustomDrawWidget::mouseReleaseEvent(QMouseEvent * pe)
{
    if (pe->button() == Qt::RightButton)
    {
        layMove = false;
    }
}
void CustomDrawWidget::wheelEvent(QMouseEvent * pe)
{

}

void CustomDrawWidget::clearVector()
{
    vecPoint.clear();
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


