#include <QSlider>
#include <QKeyEvent>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QDirIterator>

#include "window.h"
#include "mainwindow.h"

Window::Window(MainWindow *mw, int step, int shininess)
    : mainWindow(mw), step_(step), shininess_(shininess) {

    glWidget = new GLWidget(step);
    //glWidget->setFormat(QSurfaceFormat::samples());

  QGroupBox *controllersGroup = createParameterSliders();

  QVBoxLayout *mainLayout = new QVBoxLayout;
  QGridLayout *container = new QGridLayout;
  QGroupBox *bakerGroup = createBakeTypeBoxes ();

  QGroupBox *gammaGroup = createGammaSliders();

  container->addWidget(glWidget, 0, 0, 4, 1);
  container->addWidget(bakerGroup, 0,1);
  container->addWidget(gammaGroup, 2,1);
  container->addWidget(controllersGroup, 3,1);
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

  albedoSlider = createSlider(0, 100);
  albedoLabel = new QLabel(tr("Zoom:"));
  controllersLayout->addWidget(albedoLabel, currentRow, 0);
  controllersLayout->addWidget(albedoSlider, currentRow, 1);
  albedoSlider->setValue(50);
  glWidget->albedo = albedo_factor * 0.5;
  connect(albedoSlider, SIGNAL(valueChanged(int)), this, SLOT(valueAl(int)));
  currentRow++;

  textureSizeSlider = createSlider(0, 10);
  textureSizeLabel = new QLabel(tr("Atlas dimension:"));
  controllersLayout->addWidget(textureSizeLabel, currentRow, 0);
  controllersLayout->addWidget(textureSizeSlider, currentRow, 1);
  textureSizeSlider->setValue(5);
  connect(textureSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(valueAD(int)));
  currentRow++;

  textureResolutionSlider = createSlider(0, 10);
  textureResolutionLabel = new QLabel(tr("Atlas resolution:"));
  controllersLayout->addWidget(textureResolutionLabel, currentRow, 0);
  controllersLayout->addWidget(textureResolutionSlider, currentRow, 1);
  textureResolutionSlider->setValue(5);
  connect(textureResolutionSlider, SIGNAL(valueChanged(int)), this, SLOT(valueAR(int)));
  currentRow++;

  controllersGroup->setLayout(controllersLayout);
  return controllersGroup;
}
QGroupBox* Window::createGammaSliders() {
  QGroupBox *gammaGroup = new QGroupBox(tr("Anti aliasing:"));
  QGridLayout *gammaLayout = new QGridLayout;
  int currentRow = 0;

  gammaSlider = createSlider(0, 10);
  gammaLabel = new QLabel(tr("Gamma:"));
  gammaLayout->addWidget(gammaLabel, currentRow, 0);
  gammaLayout->addWidget(gammaSlider, currentRow, 1);
  gammaSlider->setValue(5);
  glWidget->set_gamma_value (0.05);
  connect(gammaSlider, SIGNAL(valueChanged(int)), this, SLOT(valueGa(int)));
  currentRow++;

  gammaGroup->setLayout(gammaLayout);
  return gammaGroup;
}

void Window::disconnectWidgetUpdate() {
  glWidget->disconnectUpdate();
}

void Window::connectWidgetUpdate() {
  glWidget->connectUpdate();
}

void Window::change_font (QString new_font_path) {
    glWidget->Text->define_font_type(new_font_path);
    glWidget->change_render = true;
}

QSlider *Window::createSlider(int min, int max) {
  QSlider *slider = new QSlider(Qt::Horizontal);
  slider->setRange(min, max);
  slider->setSingleStep(1);
  slider->setTickPosition(QSlider::TicksRight);
  return slider;
}

QGroupBox* Window::createBakeTypeBoxes() {
  QGroupBox *bakeGroup = new QGroupBox(tr("Bake type:"));
  QGroupBox *trivialGroup = new QGroupBox(tr("Trivial type:"));
  QGroupBox *distanceGroup = new QGroupBox(tr("Distance Transform:"));
  QGroupBox *curveGroup = new QGroupBox(tr("Curve:"));
  QVBoxLayout *bakeLayout = new QVBoxLayout();
  QVBoxLayout *trivialLayout = new QVBoxLayout();
  QVBoxLayout *distanceLayout = new QVBoxLayout();
  QVBoxLayout *curveLayout = new QVBoxLayout();

  trivial_texture = new QPushButton ("Trivial Texture", this);
  trivial_mip = new QPushButton ("MipMap Texture", this);
  distance_city_block = new QPushButton ("City Block", this);
  distance_chess_board = new QPushButton ("Chessboard", this);
  distance_euclidean = new QPushButton ("Euclidean", this);
  distance_fmm = new QPushButton ("Fast Marching", this);
  curve = new QPushButton ("Curve Outline", this);
  curve_blinn = new QPushButton ("Blinn-Loop", this);

  trivialLayout->addWidget(trivial_texture);
  trivialLayout->addWidget(trivial_mip);
  connect(trivial_texture, SIGNAL(clicked()), this, SLOT(trivialTexSlot ()));
  connect(trivial_mip, SIGNAL(clicked()), this, SLOT(trivialMipSlot ()));
  trivialGroup->setLayout(trivialLayout);
  bakeLayout->addWidget(trivialGroup);

  distanceLayout->addWidget(distance_city_block);
  distanceLayout->addWidget(distance_chess_board);
  distanceLayout->addWidget(distance_euclidean);
  distanceLayout->addWidget(distance_fmm);
  connect(distance_city_block, SIGNAL(clicked()), this, SLOT(distanceCityBlockSlot()));
  connect(distance_chess_board, SIGNAL(clicked()), this, SLOT(distanceChessBoardSlot()));
  connect(distance_euclidean, SIGNAL(clicked()), this, SLOT(distanceEuclideanSlot()));
  connect(distance_fmm, SIGNAL(clicked()), this, SLOT(distanceFMMSlot()));
  distanceGroup->setLayout(distanceLayout);
  bakeLayout->addWidget(distanceGroup);

  curveLayout->addWidget(curve);
  curveLayout->addWidget(curve_blinn);
  connect(curve, SIGNAL(clicked()), this, SLOT(curveOutlineSlot()));
  connect(curve_blinn, SIGNAL(clicked()), this, SLOT(curveBlinnSlot()));
  curveGroup->setLayout(curveLayout);
  bakeLayout->addWidget(curveGroup);

  trivial_texture->click();

  bakeGroup->setLayout(bakeLayout);
  return bakeGroup;
}

void Window::valueAl(int nv) {
    glWidget->albedo = albedo_factor * float(nv)/step_;
    glWidget->changed = true;
}

void Window::valueGa(int nv) {
    glWidget->set_gamma_value (gamma_factor * nv);
}

void Window::valueAD (int nv) {
    glWidget->set_atlas_dimension_value (atlas_factor * (nv+3));
}
void Window::valueAR (int nv) {
    glWidget->set_atlas_resolution_value (resolution_factor * (nv+3));
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
   glWidget->set_transform_type ("chessboard");
}

void Window::distanceEuclideanSlot () {
   glWidget->set_bake_type ("texture distance transform");
   glWidget->set_transform_type ("euclidean");
}

void Window::distanceFMMSlot () {
   glWidget->set_bake_type ("texture distance transform");
   glWidget->set_transform_type ("fast marching");
}

void Window::curveOutlineSlot () {
   glWidget->set_bake_type ("curve outline");
}
void Window::curveBlinnSlot () {
   glWidget->set_bake_type ("curve blinn-loop");
}

void Window::curveSlot () {
   qDebug () << "curve";
}

