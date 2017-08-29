#ifndef WINDOW_H
#define WINDOW_H

#include <QLabel>
#include <QWidget>
#include <QString>
#include <QCheckBox>
#include <QGroupBox>
#include <QTabWidget>
#include <QRadioButton>

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
    void load(int indSize, int vertCount, uint *inds, QVector3D *verts, QVector3D *norms);

    // Ambient Color Sliders
    QSlider *redAmbientSlider;
    QSlider *greenAmbientSlider;
    QSlider *blueAmbientSlider;
    QLabel *redAmbientLabel;
    QLabel *greenAmbientLabel;
    QLabel *blueAmbientLabel;

private slots:
    void valueRA(int);
    void valueGA(int);
    void valueBA(int);

    void valueRD(int);
    void valueGD(int);
    void valueBD(int);

    void valueRS(int);
    void valueGS(int);
    void valueBS(int);

    void valueSh(int);
    void valueAl(int);
    void valueRg(int);

    void valueShBF(bool);
    void valueShBP(bool);
    void valueShBG(bool);

    void valueRmON(bool);
    void valueRmBF(bool);

private:
    QSlider *createSlider();
    QSlider *createShininessSlider();

    float albedo_factor = 10.0;

    int step_;
    int shininess_;
    GLWidget *glWidget;
    MainWindow *mainWindow;
    QRadioButton *blinn, *nayar;
    QRadioButton *flat, *gouraud, *phong;

    // Sliders
    QSlider *albedoSlider;
    QSlider *roughnessSlider;
    QSlider *shininessSlider;
    QSlider *ambientColorSlider;
    QSlider *specularColorSlider;
    QSlider *diffusiveColorSlider;


    // Diffusive Color Sliders
    QSlider *redDiffusiveSlider;
    QSlider *greenDiffusiveSlider;
    QSlider *blueDiffusiveSlider;
    QLabel *redDiffusiveLabel;
    QLabel *greenDiffusiveLabel;
    QLabel *blueDiffusiveLabel;


    // Specular Color Sliders
    QSlider *redSpecularSlider;
    QSlider *greenSpecularSlider;
    QSlider *blueSpecularSlider;
    QLabel *redSpecularLabel;
    QLabel *greenSpecularLabel;
    QLabel *blueSpecularLabel;

    // Labels
    QLabel *albedoLabel;
    QLabel *roughnessLabel;
    QLabel *shininessLabel;
    QLabel *ambientColorLabel;
    QLabel *specularColorLabel;
    QLabel *diffusiveColorLabel;

    QWidget* createAmbientColors();
    QWidget* createSpecularColors();
    QWidget* createDiffusiveColors();
    QTabWidget* createColorSliders();
    QGroupBox* createParameterSliders();
    QGroupBox* createShadingRadialBoxes();
    QGroupBox* createReflectanceRadialBoxes();
};

#endif
