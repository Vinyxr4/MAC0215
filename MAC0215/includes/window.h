#ifndef WINDOW_H
#define WINDOW_H

#include <QLabel>
#include <QWidget>
#include <QString>
#include <QCheckBox>
#include <QGroupBox>
#include <QTabWidget>
#include <QRadioButton>

#include "glwidget.h"

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
QT_END_NAMESPACE

class GLWidget;
class MainWindow;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(MainWindow *mw, int step, int shininess);
    void connectWidgetUpdate();
    void disconnectWidgetUpdate();
    void change_font (QString new_font_path);

private slots:
    void valueAl(int);
    void valueGa(int);
    void valueAD(int);
    void valueAR(int);
    void trivialTexSlot ();
    void trivialMipSlot ();
    void distanceCityBlockSlot ();
    void distanceEuclideanSlot ();
    void distanceChessBoardSlot ();
    void distanceFMMSlot ();
    void curveOutlineSlot ();
    void curveBlinnSlot ();
    void curveSlot ();

private:
    QSlider *createSlider(int min, int max);
    QGroupBox* createBakeTypeBoxes();

    float albedo_factor = 100.0;
    float gamma_factor = 0.01;
    float atlas_factor = 0.1, resolution_factor = 0.1;

    MainWindow *mainWindow;
    int step_;
    int shininess_;
    GLWidget *glWidget;

    QPushButton *distance_chess_board, *distance_city_block, *distance_euclidean;
    QPushButton *distance_fmm;
    QPushButton *trivial_texture, *trivial_mip;
    QPushButton *curve, *curve_blinn;

    // Sliders
    QSlider *albedoSlider;
    QSlider *textureSizeSlider, *textureResolutionSlider;
    QSlider *gammaSlider;

    // Labels
    QLabel *albedoLabel;
    QLabel *textureSizeLabel, *textureResolutionLabel;
    QLabel *gammaLabel;

    QGroupBox* createParameterSliders();
    QGroupBox* createGammaSliders();
};

#endif
