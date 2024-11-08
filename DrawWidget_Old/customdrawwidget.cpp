#include "customdrawwidget.h"
#include <QPainter>
#include <iostream>

CustomDrawWidget::CustomDrawWidget(QWidget * parent) : QWidget(parent)
{
    QPoint p(100,200);
    m_point = p;
    m_color = Qt::red;

}

void CustomDrawWidget::paintEvent(QPaintEvent *event)
{
    if (vecPoint.size() > 1)
    {
        QPainter p(this);
        p.setPen(m_color);
        for (auto p_temp : vecPoint)
        {
            p.drawEllipse(p_temp.x(),p_temp.y(),20,20);
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
        std::cout << "MousePressed: x_pos - " << pe->pos().x() << " y_pos - "
                  << pe->pos().y() << std::endl;
        vecPoint.push_back( pe->pos());

        std::cout << "Send signal" << std::endl;
        emit dataChanged(pe->pos());
    }
    update();
}

void CustomDrawWidget::mouseMoveEvent(QMouseEvent * pe)
{

}
void CustomDrawWidget::mouseReleaseEvent(QMouseEvent * pe)
{

}
void CustomDrawWidget::wheelEvent(QMouseEvent * pe)
{

}
