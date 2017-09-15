QT += widgets

CONFIG += c++11

TARGET = mac0215
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -Wsign-compare

SOURCES += main.cpp \
    mainwindow.cpp \
    window.cpp \
    glwidget.cpp \
    camera3d.cpp \
    input.cpp \
    transform3d.cpp \
    glyph.cpp \
    text.cpp \
    pdf_extractor.cpp \
    distance_transform.cpp

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
    text.h \
    pdf_extractor.h \
    distance_transform.h

LIBS += -lz -lfreetype -lpython2.7
INCLUDEPATH += /usr/include/freetype2
INCLUDEPATH += /usr/include/python2.7

RESOURCES += \
    resource.qrc

DISTFILES += \
    shaders/texture.vert \
    shaders/texture.vert

