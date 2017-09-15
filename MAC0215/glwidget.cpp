#include "glwidget.h"
#include <QDebug>
#include <QString>

#include <cmath>

#include <iostream>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>

#include "input.h"
#include "text.h"

QOpenGLTexture *texture;

text *Text;
QString atlas = "teste.png";

GLWidget::GLWidget(int step, QWidget *parent)
    : QOpenGLWidget(parent),
      m_program(0) {
  step_ = step;


  Text = new text ("/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf", atlas);
  //Text->bake_dist_transf ("city_block");
  Text->bake_atlas ();
  Text->gen_test_pdf ();
}

void GLWidget::LoadText (int layers) {
    QVector3D *font_vertex = (QVector3D*) malloc (Text->font_vertices.size () * sizeof (QVector3D));
    QVector2D *font_tex = (QVector2D*) malloc (Text->font_texture.size () * sizeof (QVector2D));

    for (uint i = 0; i < Text->font_vertices.size (); ++i) {
        font_vertex[i] = Text->font_vertices[i];
        font_tex[i] = Text->font_texture[i];
    }

    delete texture;
    texture = new QOpenGLTexture (QImage (atlas).mirrored());

    //texture->setMipLevels(layers);

    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::MirroredRepeat);

    texture->generateMipMaps();

    m_object.bind();
    m_vertex.bind();

    sg_vertexes_ = font_vertex;
    vertexCount_ = Text->font_vertices.size ();
    m_vertex.allocate(sg_vertexes_, vertexCount_ * sizeof (QVector3D));

    m_object.release();
    m_vertex.release();

    m_object.bind();
    m_tex.bind();

    sg_texture_ = font_tex;
    m_tex.allocate(sg_texture_, vertexCount_ * sizeof (QVector2D));

    m_object.release();
    m_tex.release();
}

GLWidget::~GLWidget() {
    cleanup();
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(200, 200);
}

QSize GLWidget::sizeHint() const {
    return QSize(400, 400);
}

void GLWidget::cleanup() {
    makeCurrent();
    delete m_program;
    m_program = 0;
    doneCurrent();
}

void GLWidget::connectUpdate() {
  m_connection = connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
  update();
}

void GLWidget::disconnectUpdate() {
  disconnect(m_connection);
}

void GLWidget::initializeGL() {
  m_camera.translate(QVector3D(150, 250, 200));

  initializeOpenGLFunctions();
  connectUpdate();
  printContextInformation();
  // Set global information
  glEnable(GL_DEPTH_TEST);
  glDepthRange(0,1);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // Application-specific initialization
  {
    // Create Shader (Do not release until VAO is created)
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/texture.vert");
    shader = new QOpenGLShader(QOpenGLShader::Fragment);
    shader->compileSourceFile(":/shaders/texture.frag");
    m_program->addShader(shader);
    m_program->link();
    m_program->bind();

    // Cache Uniform Locations
    u_modelToWorld = m_program->uniformLocation("modelToWorld");
    u_worldToCamera = m_program->uniformLocation("worldToCamera");
    u_cameraToView = m_program->uniformLocation("cameraToView");
    m_texAttr = m_program->attributeLocation("texCoord");

    // Create Buffer (Do not release until VAO is created)
    m_vertex.create();
    m_vertex.bind();
    m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertex.allocate(sg_vertexes_, vertexCount_*sizeof(QVector3D));

    // Create Vertex Array Object
    m_object.create();
    m_object.bind();

    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3,0);
    m_program->enableAttributeArray(0);

    m_object.release();

    m_object.bind();
    m_tex.create();
    m_tex.bind();
    m_tex.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_tex.allocate(sg_texture_, vertexCount_*sizeof(QVector2D));

    m_program->setAttributeBuffer(1, GL_FLOAT, 0, 2,0);
    m_program->enableAttributeArray(1);

    m_object.release();

    last_albedo = albedo;

    LoadText(5);
    glViewport(0, 0, 10, 10);
  }
}

void GLWidget::update() {
  Input::update();
  if (Input::buttonPressed(Qt::LeftButton)) {
      m_transform.translate(Input::mouseDelta().x(), -Input::mouseDelta().y(), 0);
  }

  float scale = 0.0083;
  if (albedo < last_albedo) scale *= -1;
  if (zoom_factor + scale >+ 1) {
      changed = false;
      return;
  }
  if (changed) {
      changed = false;
      //m_transform.translate(0, 0, scale);
      zoom_factor += scale;
      resizeGL(w_size, h_size);
      last_albedo = albedo;
  }

  QOpenGLWidget::update();
}

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_program->bind();

  m_program->setUniformValue(u_worldToCamera, m_camera.toMatrix());
  m_program->setUniformValue(u_cameraToView, m_projection);
  m_program->setUniformValue("texture", 0);
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_object.bind();
    m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
    texture->bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount_);
    m_object.release();
  }
  m_program->release();
}

void GLWidget::resizeGL(int w, int h) {
  float zoom = (1 - zoom_factor);

  w_size = float(w);
  h_size = float(h);
  m_projection.setToIdentity();
  m_projection.perspective(180.0f * zoom, w / float(h), 1.0f, 1000.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  Input::registerMousePress(event->button());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  Input::registerMouseRelease(event->button());
}

void GLWidget::printContextInformation() {
  // qPrintable() will print our QString w/o quotes around it.
  qDebug() << "Context valid: " << context()->isValid();
  qDebug() << "Really used OpenGl: " << context()->format().majorVersion() << "." << context()->format().minorVersion();
  qDebug() << "OpenGl information: VENDOR:       " << (const char*)glGetString(GL_VENDOR);
  qDebug() << "                    RENDERER:    " << (const char*)glGetString(GL_RENDERER);
  qDebug() << "                    VERSION:      " << (const char*)glGetString(GL_VERSION);
  qDebug() << "                    GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}
