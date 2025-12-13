QT += core gui widgets concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    objects.cpp \
    qraytracingwidget.cpp \
    scene.cpp

HEADERS += \
    mainwindow.h \
    objects.h \
    qraytracingwidget.h \
    scene.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

mac {
    # Qt на mac иногда тащит AGL вместе с OpenGL (привет из 2009-го)
    QMAKE_LIBS_OPENGL -= -framework AGL
    LIBS             -= -framework AGL
    QMAKE_LFLAGS     -= -framework AGL
}
