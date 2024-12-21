QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bezier_curves
TEMPLATE = app


CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
    bezierutils.cpp \
    customdrawwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    longdouble.cpp

HEADERS += \
    bezierutils.h \
    customdrawwidget.h \
    mainwindow.h \
    longdouble.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
