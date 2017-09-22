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
#include "glwidget.h"
#include "mainwindow.h"

Window::Window(MainWindow *mw, int step, int shininess)
    : mainWindow(mw), step_(step), shininess_(shininess) {
  glWidget = new GLWidget(step);

  QGroupBox *controllersGroup = createParameterSliders();

  QVBoxLayout *mainLayout = new QVBoxLayout;
  QGridLayout *container = new QGridLayout;
  QGroupBox *bakerGroup = createBakeTypeBoxes ();

  container->addWidget(glWidget, 0, 0, 4, 1);
  container->addWidget(bakerGroup, 0,1);
  container->addWidget(controllersGroup, 4,1,1,2);
  container->setColumnStretch(0, 10);

  QWidget *centralWidget = new QWidget;
  centralWidget->setLayout(container);
  mainLayout->addWidget(centralWidget);

  setLayout(mainLayout);
}

QGroupBox* Window::createParameterSliders() {
  QGroupBox *controllersGroup = new QGroupBox(tr("Controls:"));
  QGridLayout *controllersLayout = new QGridLayout;
  int currentRow = 0;

  albedoSlider = createSlider();
  albedoLabel = new QLabel(tr("Zoom:"));
  controllersLayout->addWidget(albedoLabel, currentRow, 0);
  controllersLayout->addWidget(albedoSlider, currentRow, 1);
  albedoSlider->setValue(50);
  glWidget->albedo = albedo_factor * 0.5;
  connect(albedoSlider, SIGNAL(valueChanged(int)), this, SLOT(valueAl(int)));
  currentRow++;

  controllersGroup->setLayout(controllersLayout);
  return controllersGroup;
}

void Window::disconnectWidgetUpdate() {
  glWidget->disconnectUpdate();
}

void Window::connectWidgetUpdate() {
  glWidget->connectUpdate();
}

QSlider *Window::createSlider() {
  QSlider *slider = new QSlider(Qt::Horizontal);
  slider->setRange(0, 100);
  slider->setSingleStep(1);
  slider->setTickPosition(QSlider::TicksRight);
  return slider;
}

QGroupBox* Window::createBakeTypeBoxes() {
  QGroupBox *bakeGroup = new QGroupBox(tr("Bake type:"));
  QVBoxLayout *bakeLayout = new QVBoxLayout();

  trivial = new QRadioButton ("Traditional", this);
  distance_transform = new QRadioButton ("Distance Transform", this);

  bakeLayout->addWidget(trivial);
  bakeLayout->addWidget(distance_transform);

  connect(trivial, SIGNAL(clicked(bool)), this, SLOT(bakeSlot (bool)));
  connect(distance_transform, SIGNAL(clicked(bool)), this, SLOT(distanceTransformSlot(bool)));

  distance_transform->click();

  bakeGroup->setLayout(bakeLayout);
  return bakeGroup;
}

void Window::valueAl(int nv) {
    glWidget->albedo = albedo_factor * float(nv)/step_;
    glWidget->changed = true;
}

// Bake type button controller
void Window::bakeSlot (bool nv) {
    if (nv == true) {
        qDebug () << "ok";
       glWidget->set_bake_type ("trivial");
       glWidget->set_transform_type ("");
    }
}

void Window::distanceTransformSlot (bool nv) {
    if (nv == true) {
       glWidget->set_bake_type ("texture distance transform");
       glWidget->set_transform_type ("city_block");
    }
}
