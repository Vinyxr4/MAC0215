#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QString>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLWindow>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

#include "camera3d.h"
#include "transform3d.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(int step, QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void connectUpdate();
    void disconnectUpdate();
    void LoadText (int layers);

    float albedo, last_albedo;
    bool changed = false;

    QOpenGLShaderProgram *m_program;
    QOpenGLShader *shader;

public slots:
    void cleanup();

protected:
    void paintGL() override;
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


protected slots:
  void update();

private:
    void printContextInformation();

    QOpenGLBuffer m_vertex;
    QOpenGLBuffer m_normal;
    QOpenGLBuffer m_tex;

    QOpenGLVertexArrayObject m_object;

    // Shader Information
    int u_modelToWorld;
    int u_cameraToView;
    int u_worldToCamera;

    Camera3D m_camera;
    QMatrix4x4 m_projection;
    Transform3D m_transform;

    int step_;
    int indiceSize_;
    int vertexCount_;
    int m_texAttr;
    GLuint* indices_;
    QVector3D* sg_vertexes_;
    QVector3D *vertices;
    QVector2D *Texture, *sg_texture_;
    GLuint *indices;

    float dist_to_pages = -100.0;
    float zoom_factor = 0.65;
    float w_size, h_size;

    QMetaObject::Connection m_connection;

};

#endif
