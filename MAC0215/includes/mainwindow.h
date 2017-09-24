#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "window.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
    Window* window_;
    QStringList fonts;

    int current_font_to_add;
    void change_font (int i);
};

#endif
