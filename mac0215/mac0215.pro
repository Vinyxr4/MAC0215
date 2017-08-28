#-------------------------------------------------
#
# Project created by QtCreator 2017-08-28T16:13:54
#
#-------------------------------------------------

QT += widgets
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mac0215
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    text.cpp \
    transform3d.cpp \
    window.cpp \
    input.cpp \
    camera3d.cpp

HEADERS  += mainwindow.h \
    text.h \
    camera3d.h \
    transform3d.h \
    window.h \
    input.h

FORMS    += mainwindow.ui

LIBS += -lz -lfreetype

INCLUDEPATH += /usr/include/freetype2

RESOURCES += \
    resources.qrc
