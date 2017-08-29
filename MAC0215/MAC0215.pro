QT += widgets

CONFIG += c++11

TARGET = pa2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    window.cpp \
    glwidget.cpp \
    camera3d.cpp \
    input.cpp \
    transform3d.cpp \
    glyph.cpp \
    text.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

qmakeforce.target = dummy
qmakeforce.commands = rm -f Makefile ##to force rerun of qmake
qmakeforce.depends = FORCE
PRE_TARGETDEPS += $$qmakeforce.target
QMAKE_EXTRA_TARGETS += qmakeforce

HEADERS += \
    mainwindow.h \
    window.h \
    glwidget.h \
    camera3d.h \
    input.h \
    transform3d.h \
    glyph.h \
    text.h

LIBS += -lz -lfreetype
INCLUDEPATH += /usr/include/freetype2

RESOURCES += \
    resource.qrc

DISTFILES += \
    shaders/BP_noInterp.vert \
    shaders/BP_noInterp.frag \
    shaders/BP_interp.vert \
    shaders/BP_interp.frag \
    shaders/ON_interp.vert \
    shaders/ON_interp.frag \
    shaders/ON_noInterp.vert \
    shaders/ON_noInterp.frag \

