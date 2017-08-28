#ifndef TEXT_H
#define TEXT_H

#include <QMainWindow>
#include <QString>
#include <QKeyEvent>
#include <QDebug>
#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <QOpenGLWindow>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <ft2build.h>
#include FT_FREETYPE_H

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

#include "camera3d.h"
#include "transform3d.h"

class text: public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    text(QString font, QWidget *parent = 0);

    void LoadCube();
    void define_font_type (QString font);
    void define_text (QString t);
    void bake_atlas ();
    void render_text ();
    void connectUpdate();

    QOpenGLShaderProgram *m_program;
    QOpenGLShader *shader;

protected:
    void paintGL() override;
    void initializeGL () override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

protected slots:
  void update();

private:
    QString font_path;
    QString text_to_render;
    FT_Library ft;
    FT_Face face;

    QOpenGLBuffer m_vertex;
    QOpenGLBuffer m_normal;
    QOpenGLVertexArrayObject m_object;

    // Shader
    int u_modelToWorld;
    int u_cameraToView;
    int u_worldToCamera;
    int u_lightPosition;

    int m_normalAttr;

    Camera3D m_camera;
    QMatrix4x4 m_projection;
    Transform3D m_transform;

    QVector3D vLightPosition;
    QVector3D* sg_vertexes_, *sg_normals_;
    int vertexCount_, normalCount_;
    int indiceSize_, normalIndiceSize_;
    GLuint* indices_, *normalIndices_;
    QVector3D *vertices, *normals, *flatNormals, *meanNormals;
    GLuint *indices, *normalIndices;

    QMetaObject::Connection m_connection;
};

#endif // TEXT_H
