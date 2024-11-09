#include "task_1.h"
#include "mathfunctions.h"
#include <QPainter>
#include <QMessageBox>

Task1Widget::Task1Widget(QWidget *parent) : QWidget(parent) {}

void Task1Widget::mousePressEvent(QMouseEvent *event)
{
    if (isSettingFirstSegment) {
        if (firstPointA.isNull()) {
            firstPointA = event->pos();
        } else if (secondPointA.isNull()) {
            secondPointA = event->pos();
            isSettingFirstSegment = false;
        }
    } else {
        if (firstPointB.isNull()) {
            firstPointB = event->pos();
        } else if (secondPointB.isNull()) {
            secondPointB = event->pos();
            isSettingFirstSegment = true;

            // Проверка пересечения после задания всех точек
            bool intersect = doIntersect(firstPointA, secondPointA, firstPointB, secondPointB);
            QString message = intersect ? "Отрезки пересекаются." : "Отрезки не пересекаются.";
            QMessageBox::information(this, "Результат пересечения", message);
        }
    }
    update();
}

void Task1Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton) {
        QPoint *pointToMove = nullptr;

        if ((firstPointA - event->pos()).manhattanLength() < 10) {
            pointToMove = &firstPointA;
        } else if ((secondPointA - event->pos()).manhattanLength() < 10) {
            pointToMove = &secondPointA;
        } else if ((firstPointB - event->pos()).manhattanLength() < 10) {
            pointToMove = &firstPointB;
        } else if ((secondPointB - event->pos()).manhattanLength() < 10) {
            pointToMove = &secondPointB;
        }

        if (pointToMove) {
            *pointToMove = event->pos();
            update();
            emit pointsMoved();
        }
    }
}

void Task1Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::blue);
    if (!firstPointA.isNull() && !secondPointA.isNull()) {
        painter.drawLine(firstPointA, secondPointA);
    }
    if (!firstPointB.isNull() && !secondPointB.isNull()) {
        painter.drawLine(firstPointB, secondPointB);
    }
}

void Task1Widget::resetSegments()
{
    firstPointA = QPoint();
    secondPointA = QPoint();
    firstPointB = QPoint();
    secondPointB = QPoint();
    isSettingFirstSegment = true;
    update();
}
