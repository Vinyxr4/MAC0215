#include "mainwindow.h"
#include "window.h"

#include <QMenu>
#include <QDebug>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QVector3D>
#include <vector>

MainWindow::MainWindow() {
  window_ = new Window(this, 100, 64);
  setCentralWidget(window_);
  setWindowTitle("MAC0215");
}
