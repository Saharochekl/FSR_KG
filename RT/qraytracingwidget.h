#ifndef QRAYTRACINGWIDGET_H
#define QRAYTRACINGWIDGET_H

#include <QWidget>
#include "scene.h"

class QRayTracingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QRayTracingWidget(QWidget *parent = nullptr);

    virtual void paintEvent(QPaintEvent *);

    Scene cur_sc;

    void MakeFilm();



signals:

private slots:
    void onTick();
    void onFrameReady(const QImage& img);

private:
    QImage frame;
    bool renderInProgress = false;
    QTimer* timer = nullptr;


};

#endif // QRAYTRACINGWIDGET_H
