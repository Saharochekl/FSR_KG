#include "customdrawwidget.h"
#include <QPainter>
#include <iostream>

CustomDrawWidget::CustomDrawWidget(QWidget * parent) : QWidget(parent)
{
    m_color = Qt::red;

    scale = 10;

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
            painter.drawEllipse(vecPoint[0].x() * scale, vecPoint[0].y() * scale, 5,5);
            break;
        case 2:
            painter.drawLine(vecPoint[0] * scale, vecPoint[1] * scale);
            break;
        case 3:
            painter.drawLine(vecPoint[0] * scale, vecPoint[1] * scale);
            painter.drawEllipse(vecPoint[2].x() * scale, vecPoint[2].y() * scale, 5,5);
            break;
        case 4:
            painter.drawLine(vecPoint[0] * scale, vecPoint[1] * scale);
            painter.drawLine(vecPoint[2] * scale, vecPoint[3] * scale);
            break;
        }
    }
    else if (typeTask == task2)
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
            painter.drawEllipse(vecPoint[0].x() * scale, vecPoint[0].y() * scale, 5,5);
            break;
        case 2:
            painter.drawLine(vecPoint[0] * scale, vecPoint[1] * scale);
            break;
        case 3:
            painter.drawLine(vecPoint[0] * scale, vecPoint[1] * scale);
            painter.drawEllipse(vecPoint[2].x() * scale, vecPoint[2].y() * scale, 5,5);
            break;
        }

    }
    else if (typeTask == task3)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPen pen;
        pen.setColor(m_color);
        pen.setWidth(1.5);
        painter.setPen(pen);
        for (auto point : vecPoint)
        {
            painter.drawEllipse(point.x() * scale, point.y() * scale, 5,5);
        }
    }
    else if (typeTask == task5)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPen pen;
        pen.setColor(m_color);
        pen.setWidth(1.5);
        painter.setPen(pen);

        for (auto point : Polygon1)
        {
            painter.drawEllipse(point.x() * scale, point.y() * scale, 5,5);
        }

        QPen pen1;
        pen1.setColor(Qt::blue);
        pen1.setWidth(1.5);
        painter.setPen(pen1);
        for (auto point : Polygon2)
        {
            painter.drawEllipse(point.x() * scale, point.y() * scale, 5,5);
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
        if (typeTask == task1){
            if (vecPoint.size() > 4)
            {
                vecPoint.clear();
            }
            vecPoint.push_back( pe->localPos()/scale);
        }
        if (typeTask == task2){
            if (vecPoint.size() > 3)
            {
                vecPoint.clear();
            }
            vecPoint.push_back( pe->localPos()/scale);
        }
        if (typeTask == task5)
        {
            if (isFirstPolygon == true)
            {
                Polygon1.push_back( pe->localPos()/scale);
            }
            else
            {
                Polygon2.push_back( pe->localPos()/scale);
            }


        }

        emit dataChanged(pe->localPos()/scale);
    }
    else if (pe->button() == Qt::RightButton)
    {
        layMove = true;
        if (typeTask == task1 or typeTask == task2 or typeTask == task3)
        {
            if (vecPoint.size() > 0)
            {
                double radius = 10;
                double dist = 99999;
                int cur_id = -1;

                for (int i =0; i< vecPoint.size(); i++)
                {
                    double d = distance(vecPoint[i], pe->localPos()/scale);
                    if (d < radius)
                    {
                        if (d < dist){
                            dist = d;
                            cur_id = i;
                        }
                    }
                }
                m_id = cur_id;
            }
        }
        if (typeTask == task5)
        {

        }
    }

    update();
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
                vecPoint[m_id] = pe->localPos() / scale;
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
    repaint();

    if (typeTask == task5)
    {
        isFirstPolygon = true;
        Polygon1.clear();
        Polygon2.clear();
    }
}

void CustomDrawWidget::getResultOfTasks()
{
    if (typeTask == task1)
    {

    }
    if (typeTask == task2)
    {

    }
    if (typeTask == task3)
    {

    }
    if (typeTask == task4)
    {

    }
    if (typeTask == task5)
    {

    }
    if (typeTask == task6)
    {

    }

}
