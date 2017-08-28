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

class text;
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

private:

    MainWindow *mainWindow;
    int step_;
    int shininess_;
    text *textWidget;
};

#endif
