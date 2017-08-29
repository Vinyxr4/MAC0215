#include <QSlider>
#include <QKeyEvent>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>

#include "window.h"
#include "mainwindow.h"
#include "text.h"

Window::Window(MainWindow *mw, int step, int shininess)
    : mainWindow(mw), step_(step), shininess_(shininess) {
  textWidget = new text (QString ("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf"));

  QVBoxLayout *mainLayout = new QVBoxLayout;
  QGridLayout *container = new QGridLayout;

  container->addWidget(textWidget, 0, 0, 4, 1);
  container->setColumnStretch(0, 10);

  QWidget *centralWidget = new QWidget;
  centralWidget->setLayout(container);
  mainLayout->addWidget(centralWidget);

  setLayout(mainLayout);
}

void Window::disconnectWidgetUpdate() {
  //glWidget->disconnectUpdate();
}

void Window::connectWidgetUpdate() {
  //glWidget->connectUpdate();
}
