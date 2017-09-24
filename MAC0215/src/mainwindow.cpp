#include "mainwindow.h"
#include "window.h"

#include <QMenu>
#include <QDebug>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QDirIterator>
#include <QVector3D>
#include <vector>
#include <QScrollArea>

MainWindow::MainWindow() {
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuWindow = menuBar->addMenu (tr("&Font"));

    QDirIterator fonts_dirs ("/usr/share/fonts/truetype", QDirIterator::Subdirectories);
    while (fonts_dirs.hasNext ()) {
        QFile f(fonts_dirs.next ());
        if (f.fileName ().endsWith (".ttf"))
          fonts << f.fileName ();
    }
    fonts.sort();

    for (int i = 0; i < fonts.size (); ++i) {
        QAction *fontAction = new QAction (menuWindow);
        fontAction->setText(fonts[i].section('/', -1));
        menuWindow->addAction (fontAction);
        connect(fontAction, &QAction::triggered, this, [this, i]{ MainWindow::change_font(i); });
    }

    //connect(openAction, &QAction::triggered, this, &MainWindow::loadFile);

    setMenuBar(menuBar);
    window_ = new Window(this, 100, 64);
    setCentralWidget(window_);
    setWindowTitle("MAC0215");
}

void MainWindow::change_font (int i) {
    window_->change_font(fonts[i]);
}
