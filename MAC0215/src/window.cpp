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
  QGroupBox *trivialGroup = new QGroupBox(tr("Trivial type:"));
  QGroupBox *distanceGroup = new QGroupBox(tr("Distance Transform:"));
  QVBoxLayout *bakeLayout = new QVBoxLayout();
  QVBoxLayout *trivialLayout = new QVBoxLayout();
  QVBoxLayout *distanceLayout = new QVBoxLayout();

  trivial_texture = new QPushButton ("Trivial Texture", this);
  trivial_mip = new QPushButton ("MipMap Texture", this);
  distance_city_block = new QPushButton ("City Block", this);
  distance_chess_board = new QPushButton ("Chessboard", this);
  distance_euclidean = new QPushButton ("Euclidean", this);

  trivialLayout->addWidget(trivial_texture);
  trivialLayout->addWidget(trivial_mip);
  connect(trivial_texture, SIGNAL(clicked()), this, SLOT(trivialTexSlot ()));
  connect(trivial_mip, SIGNAL(clicked()), this, SLOT(trivialMipSlot ()));
  trivialGroup->setLayout(trivialLayout);
  bakeLayout->addWidget(trivialGroup);

  distanceLayout->addWidget(distance_city_block);
  distanceLayout->addWidget(distance_chess_board);
  distanceLayout->addWidget(distance_euclidean);
  connect(distance_city_block, SIGNAL(clicked()), this, SLOT(distanceCityBlockSlot()));
  connect(distance_chess_board, SIGNAL(clicked()), this, SLOT(distanceChessBoardSlot()));
  connect(distance_euclidean, SIGNAL(clicked()), this, SLOT(distanceEuclideanSlot()));
  distanceGroup->setLayout(distanceLayout);
  bakeLayout->addWidget(distanceGroup);

  trivial_texture->click();

  bakeGroup->setLayout(bakeLayout);
  return bakeGroup;
}

void Window::valueAl(int nv) {
    glWidget->albedo = albedo_factor * float(nv)/step_;
    glWidget->changed = true;
}

// Bake type button controller
void Window::trivialTexSlot () {
   glWidget->set_bake_type ("trivial");
   glWidget->set_trivial_type ("texture");
}

void Window::trivialMipSlot () {
   glWidget->set_bake_type ("trivial");
   glWidget->set_trivial_type ("texture mip");
}

void Window::distanceCityBlockSlot () {
   glWidget->set_bake_type ("texture distance transform");
   glWidget->set_transform_type ("city_block");
}

void Window::distanceChessBoardSlot () {
   glWidget->set_bake_type ("texture distance transform");
   glWidget->set_transform_type ("chess_board");
}

void Window::distanceEuclideanSlot () {
   glWidget->set_bake_type ("texture distance transform");
   glWidget->set_transform_type ("euclidean");
}
