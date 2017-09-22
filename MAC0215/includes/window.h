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

private slots:
    void valueAl(int);
    void bakeSlot (bool nv);
    void distanceTransformSlot (bool nv);

private:
    QSlider *createSlider();
    QGroupBox* createBakeTypeBoxes();

    float albedo_factor = 100.0;

    MainWindow *mainWindow;
    int step_;
    int shininess_;
    GLWidget *glWidget;

    QRadioButton *trivial, *distance_transform;

    // Sliders
    QSlider *albedoSlider;

    // Labels
    QLabel *albedoLabel;

    QGroupBox* createParameterSliders();
};

#endif
