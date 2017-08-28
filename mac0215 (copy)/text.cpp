#include "text.h"
#include <QOpenGLShaderProgram>
#include <QCoreApplication>

#include "input.h"

text::text (QString font, QWidget *parent) : QOpenGLWidget(parent), m_program(0) {
    define_font_type (QString (font));

    if (FT_Init_FreeType (&ft))
        qDebug() << "ruim init";
    if(FT_New_Face(ft, font_path.toStdString().c_str(), 0, &face))
      qDebug() << "ruim new face";

    //FT_Set_Pixel_Sizes(face, 0, 48);
    //FT_Load_Char(face, 'x', FT_LOAD_RENDER);
    //qDebug () << face->glyph->bitmap.width;
}

void text::define_text (QString t) {
    text_to_render = QString (t);
}

void text::define_font_type (QString font) {
    font_path = font;
}

void text::connectUpdate() {
  m_connection = connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
  update();
}

void text::initializeGL () {
  vLightPosition = QVector3D (0.0f, 4.0f, 5.0f);


  m_camera.translate(QVector3D(0.0f, 0.0f, 2.0f));

  m_transform.translate(0.0f, 0.0f, -5.0f);

  // Initialize OpenGL Backend
  initializeOpenGLFunctions();

  connectUpdate();

  // Set global information
  glEnable(GL_DEPTH_TEST);
  glDepthRange(0,1);
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

  // Application-specific initialization
  {
    // Create Shader (Do not release until VAO is created)
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/BP_interp.vert");
    shader = new QOpenGLShader(QOpenGLShader::Fragment);
    shader->compileSourceFile(":/shaders/BP_interp.frag");
    m_program->addShader(shader);
    m_program->link();
    m_program->bind();

    // Cache Uniform Locations
    u_modelToWorld = m_program->uniformLocation("modelToWorld");
    u_worldToCamera = m_program->uniformLocation("worldToCamera");
    u_cameraToView = m_program->uniformLocation("cameraToView");
    u_lightPosition = m_program->uniformLocation("lightPosition");
    m_normalAttr = m_program->attributeLocation("vNormal");

    // Create Buffer (Do not release until VAO is created)
    m_vertex.create();
    m_vertex.bind();
    m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertex.allocate(sg_vertexes_, vertexCount_ * sizeof(QVector3D));

    // Create Vertex Array Object
    m_object.create();
    m_object.bind();

    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3,0);
    m_program->enableAttributeArray(0);

    // Release (unbind) all
    m_object.release();

    m_object.bind();
    m_normal.create();
    m_normal.bind();
    m_normal.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_normal.allocate(sg_normals_, normalCount_*sizeof(QVector3D));

    m_program->setAttributeBuffer(1, GL_FLOAT, 0, 3,0);
    m_program->enableAttributeArray(1);

    m_object.release();
    m_vertex.release();
    m_normal.release();
    m_program->release();
  }
}

void text::update() {
  Input::update();
  if (Input::buttonPressed(Qt::LeftButton)) {
    static const float rotSpeed   = 0.75f;
    m_transform.rotate(rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
    m_transform.rotate(rotSpeed * Input::mouseDelta().y(), Camera3D::LocalRight);
  }
  // Schedule a redraw
  QOpenGLWidget::update();
}

void text::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_program->bind();

  m_program->setUniformValue(u_worldToCamera, m_camera.toMatrix());

  m_program->setUniformValue(u_lightPosition, vLightPosition.toVector4D());
  m_program->setUniformValue(u_cameraToView, m_projection);
  {
    m_object.bind();
    m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
    glDrawArrays(GL_TRIANGLES, 0, vertexCount_);
    m_object.release();
  }
  m_program->release();
}

void text::mousePressEvent(QMouseEvent *event) {
  Input::registerMousePress(event->button());
}

void text::mouseReleaseEvent(QMouseEvent *event) {
  Input::registerMouseRelease(event->button());
}
