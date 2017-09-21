QT += widgets
TARGET = mac0215

CONFIG += c++11 console
CONFIG -= app_bundle
TEMPLATE = app

QMAKE_CXXFLAGS += -Wsign-compare
DEFINES += QT_DEPRECATED_WARNINGS
qmakeforce.target = dummy
qmakeforce.commands = rm -f Makefile
qmakeforce.depends = FORCE
PRE_TARGETDEPS += $$qmakeforce.target
QMAKE_EXTRA_TARGETS += qmakeforce

HEADERS += $$files(includes/*.h, true)

VPATH += src/
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

LIBS += -lz -lfreetype -lpython2.7
INCLUDEPATH += /usr/include/freetype2 /usr/include/python2.7 includes/

RESOURCES += resource.qrc
DISTFILES += shaders/texture.vert shaders/texture.vert
