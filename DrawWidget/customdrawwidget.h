#ifndef CUSTOMDRAWWIDGET_H
#define CUSTOMDRAWWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPen>

class CustomDrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomDrawWidget(QWidget * parent = nullptr);
    virtual void paintEvent(QPaintEvent *event);

    void changedColor(QColor color);


    virtual void mousePressEvent(QMouseEvent * pe);
    // работа с мышью
    virtual void mouseMoveEvent(QMouseEvent * pe);
    virtual void mouseReleaseEvent(QMouseEvent * pe);
    virtual void wheelEvent(QMouseEvent * pe);

    QVector<QPoint> vecPoint;
    QPoint m_point;
    QColor m_color;

signals:
    void dataChanged(QPoint point);
};

#endif // CUSTOMDRAWWIDGET_H
