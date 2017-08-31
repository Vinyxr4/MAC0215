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

// Create a colored cube

QVector3D cubeVertices[] = {
QVector3D( 0.5f,  0.5f,  0.5f),
QVector3D(-0.5f,  0.5f,  0.5f),
QVector3D(-0.5f, -0.5f,  0.5f),
QVector3D( 0.5f, -0.5f,  0.5f),
QVector3D( 0.5f,  0.5f, -0.5f),
QVector3D(-0.5f,  0.5f, -0.5f),
QVector3D(-0.5f, -0.5f, -0.5f),
QVector3D( 0.5f, -0.5f, -0.5f)};


static GLuint cubeIndices[] = {
0,1,2, 2,3,0,
7,5,4, 5,7,6,
0,4,5, 5,1,0,
3,2,6, 6,7,3,
2,1,5, 2,5,6,
0,3,7, 7,4,0
};


text *Text;

GLWidget::GLWidget(int step, QWidget *parent)
    : QOpenGLWidget(parent),
      m_program(0) {
  step_ = step;

  Text = new text (QString ("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf"));
  Text->gen_test();

  LoadCube();
}

void GLWidget::LoadCube() {
    vertices = (QVector3D*) malloc (36 * sizeof (QVector3D));
    normals = (QVector3D*) malloc (36 * sizeof (QVector3D));
    flatNormals = (QVector3D*) malloc (36 * sizeof (QVector3D));
    meanNormals = (QVector3D*) malloc (36 * sizeof (QVector3D));

    std::vector<QVector3D> faceNorms;
    for (int i = 0; i < (int) 36; i += 3) {
        QVector3D u(cubeVertices[cubeIndices[i]] - cubeVertices[cubeIndices[i+2]]);
        QVector3D v(cubeVertices[cubeIndices[i+1]] - cubeVertices[cubeIndices[i]]);
        QVector3D prod = u.crossProduct(u,v);
        faceNorms.push_back(prod);
    }

    for (int i = 0; i < 36;++i) {
        flatNormals[i] = faceNorms[i/3];
        flatNormals[i].normalize();
    }

    QVector3D *mean = (QVector3D*) malloc (8 * sizeof (QVector3D));
    for (int i = 0; i < 8;++i) {
        mean[i] = QVector3D(0.0f, 0.0f, 0.0f);
        for (int j = 0; j < (int) 36; ++j)
                mean[cubeIndices[j]] += faceNorms[j/3];
    }
    for (int i = 0; i < 8; ++i)
        mean[i].normalize();

    for (int i = 0; i < 36; ++i) {
        vertices[i] = cubeVertices[cubeIndices[i]];
        normals[i] = cubeVertices[cubeIndices[i]];
        normals[i].normalize();
        meanNormals[i] = mean[cubeIndices[i]];
    }
    free(mean);

    indices = (GLuint*) malloc (36 * sizeof (GLuint));
    normalIndices = (GLuint*) malloc (36 * sizeof (GLuint));

  for (int i = 0; i < 36; ++i)
      indices[i] = cubeIndices[i];

  indiceSize_ = 36;
  vertexCount_ = 36;
  indices_ = indices;
  sg_vertexes_ = vertices;

  normalIndiceSize_ = 36;
  normalCount_ = 36;
  normalIndices_ = normalIndices;
  sg_normals_ = normals;
}

void GLWidget::LoadText () {
    QVector3D *font_vertex = (QVector3D*) malloc (Text->font_vertices.size () * sizeof (QVector3D));
    QVector2D *font_tex = (QVector2D*) malloc (Text->font_texture.size () * sizeof (QVector2D));

    for (uint i = 0; i < Text->font_vertices.size (); ++i) {
        font_vertex[i] = Text->font_vertices[i];
        font_tex[i] = Text->font_texture[i];
    }

    delete texture;
    texture = new QOpenGLTexture (QImage ("atlas.png").mirrored());

    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::MirroredRepeat);

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

void GLWidget::reload () {
    m_object.bind();
    m_vertex.bind();

    sg_vertexes_ = vertices;
    vertexCount_ =36;
    m_vertex.allocate(sg_vertexes_, vertexCount_ * sizeof (QVector3D));

    m_object.release();
    m_vertex.release();
}

void GLWidget::changeToFlat() {
    m_object.bind();
    m_normal.bind();

    sg_normals_ = flatNormals;
    m_normal.allocate(sg_normals_, normalCount_*sizeof(QVector3D));

    m_object.release();
    m_normal.release();
}

void GLWidget::changeToNorm() {
    m_object.bind();
    m_normal.bind();

    sg_normals_ = normals;
    m_normal.allocate(sg_normals_, normalCount_*sizeof(QVector3D));

    m_object.release();
    m_normal.release();
}

void GLWidget::changeToMean() {
    m_object.bind();
    m_normal.bind();

    sg_normals_ = meanNormals;
    m_normal.allocate(sg_normals_, normalCount_*sizeof(QVector3D));

    m_object.release();
    m_normal.release();
}

void GLWidget::loadObj (int indSize, int vertCount, uint *inds, QVector3D *verts, QVector3D *norms) {
    m_program->bind();
    m_vertex.bind();
    m_object.bind();

    free(vertices);
    vertices = (QVector3D*) malloc (indSize * sizeof (QVector3D));
    free(normals);
    normals = (QVector3D*) malloc (indSize * sizeof (QVector3D));
    free(flatNormals);
    flatNormals = (QVector3D*) malloc (indSize * sizeof (QVector3D));
    free(meanNormals);
    meanNormals = (QVector3D*) malloc (indSize * sizeof (QVector3D));
    free (indices);
    indices = (GLuint*) malloc (indSize * sizeof (GLuint));


    QVector3D *mean = (QVector3D*) malloc (vertCount * sizeof (QVector3D));
    std::vector<QVector3D> faceNorms;
    for (int i = 0; i < (int) indSize; i += 3) {
        int x, y, z;
        QVector3D u(verts[x = inds[i]] - verts[z = inds[i+2]]);
        QVector3D v(verts[y = inds[i+1]] - verts[inds[i]]);
        QVector3D prod = u.crossProduct(u,v);
        faceNorms.push_back(prod);
        mean[x] += prod.normalized();
        mean[y] += prod.normalized();
        mean[z] += prod.normalized();
    }

    for (int i = 0; i < indSize;++i) {
        flatNormals[i] = faceNorms[i/3];
        flatNormals[i].normalize();
    }

    for (int i = 0; i < vertCount; ++i)
        mean[i].normalize();

    float min_w,min_h, max_w, max_h;
    max_w = max_h = 0.0f;
    min_w = (float) maximumWidth();
    min_h = (float) maximumHeight();

    for (int i = 0; i < indSize; ++i) {
        normals[i] = norms[inds[i]];
        meanNormals[i] = mean[inds[i]];
        vertices[i] = verts[inds[i]];
        indices[i] = (GLuint) inds[i];
        if (vertices[i].x() < min_w)
            min_w = vertices[i].x();
        else if (vertices[i].x() > max_w)
            max_w = vertices->x();
        if (vertices[i].y() < min_h)
            min_h = vertices[i].y();
        else if (vertices[i].y() > max_h)
            max_h = vertices->y();
    }
    free (mean);

    indiceSize_ = indSize;
    vertexCount_ = indSize;
    indices_ = indices;
    sg_vertexes_ = vertices;

    normalCount_ = indSize;

    if (shading == 0)
        sg_normals_ = flatNormals;
    else if (shading == 1)
        sg_normals_ = meanNormals;
    else
        sg_normals_ = normals;

    m_vertex.allocate(sg_vertexes_, vertexCount_*sizeof(QVector3D));

    float max = max_w - min_w;
    if (max < max_h - min_h)
        max = max_h - min_h;

    m_transform.translate(0.0f, 0.0f, -actual +max*factor);
    actual = max*factor;

    m_object.release();
    m_vertex.release();

    m_normal.bind();
    m_object.bind();

    m_normal.allocate(sg_normals_, normalCount_*sizeof(QVector3D));

    m_object.release();
    m_normal.release();

    m_program->release();
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
  //vLightPosition = QVector3D (0.0f, 4.0f, 5.0f);


  m_camera.translate(QVector3D(0.0f, 0.0f, 10.0f));
  // Put the object a little to the front of the camera (the camera is looking
  // at (0.0, 0.0, -1.0))
  actual = factor = -5;
  m_transform.translate(0.0f, 0.0f, -5.0f);
  // Initialize OpenGL Backend
  initializeOpenGLFunctions();
  connectUpdate();
  printContextInformation();
  // Set global information
  glEnable(GL_DEPTH_TEST);
  glDepthRange(0,1);
  glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

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
    //u_lightPosition = m_program->uniformLocation("lightPosition");
    //u_difuseColor= m_program->uniformLocation("diffuseProduct");
    //u_ambientColor= m_program->uniformLocation("ambientProduct");
    //u_specularColor= m_program->uniformLocation("specularProduct");
    //u_shininess= m_program->uniformLocation("shininess");
    //u_albedo= m_program->uniformLocation("albedo");
    //u_roughness= m_program->uniformLocation("roughness");
    //m_normalAttr = m_program->attributeLocation("vNormal");
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

    /*
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
    */
  }
}

void GLWidget::update() {
  Input::update();
  if (Input::buttonPressed(Qt::LeftButton)) {
    static const float rotSpeed   = 0.75f;
    m_transform.rotate(rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
    m_transform.rotate(rotSpeed * Input::mouseDelta().y(), Camera3D::LocalRight);
  }
  // Schedule a redraw
  QOpenGLWidget::update();
}

void GLWidget::reload_shader () {
    m_program = new QOpenGLShaderProgram();

    if (reflectmodel == 0) {
        switch (shading) {
        case 0:
            m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/BP_interp.vert");
            shader->compileSourceFile(":/shaders/BP_interp.frag");
            break;
        case 1:
            m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/BP_noInterp.vert");
            shader->compileSourceFile(":/shaders/BP_noInterp.frag");
            break;
        case 2:
            m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/BP_interp.vert");
            shader->compileSourceFile(":/shaders/BP_interp.frag");
            break;
        default:
            break;
        }
    }
    else if (reflectmodel == 1) {
        switch (shading) {
        case 0:
            m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ON_noInterp.vert");
            shader->compileSourceFile(":/shaders/ON_noInterp.frag");
            break;
        case 1:
            m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ON_noInterp.vert");
            shader->compileSourceFile(":/shaders/ON_noInterp.frag");
            break;
        case 2:
            m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ON_interp.vert");
            shader->compileSourceFile(":/shaders/ON_interp.frag");
            break;
        default:
            break;
        }
    }

    m_program->addShader(shader);
    m_program->link();
    m_program->bind();

    u_modelToWorld = m_program->uniformLocation("modelToWorld");
    u_worldToCamera = m_program->uniformLocation("worldToCamera");
    u_cameraToView = m_program->uniformLocation("cameraToView");
    u_lightPosition = m_program->uniformLocation("lightPosition");
    u_difuseColor= m_program->uniformLocation("diffuseProduct");
    u_ambientColor= m_program->uniformLocation("ambientProduct");
    u_specularColor= m_program->uniformLocation("specularProduct");
    u_shininess= m_program->uniformLocation("shininess");
    u_albedo= m_program->uniformLocation("albedo");
    u_roughness= m_program->uniformLocation("roughness");
    m_normalAttr = m_program->attributeLocation("vNormal");

    m_program->release();
}

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*
  if (changed) {
    reload_shader();
    changed = false;
  }
 */
  m_program->bind();

  m_program->setUniformValue(u_worldToCamera, m_camera.toMatrix());

  //m_program->setUniformValue(u_lightPosition, vLightPosition.toVector4D());
  m_program->setUniformValue(u_cameraToView, m_projection);
  //m_program->setUniformValue(u_difuseColor, difusiveColor);
  //m_program->setUniformValue(u_ambientColor, ambientColor);
  //m_program->setUniformValue(u_specularColor, specularColor);
  //m_program->setUniformValue(u_shininess, shininess);
  //m_program->setUniformValue(u_albedo, albedo);
  //m_program->setUniformValue(u_roughness, roughness);
  m_program->setUniformValue("texture", 0);
  /*{
    m_object.bind();
    m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
    glDrawArrays(GL_TRIANGLES, 0, vertexCount_);
    m_object.release();
  }*/
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    LoadText ();
    m_object.bind();
    m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
    texture->bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount_);
    m_object.release();
    //reload ();
  }
  m_program->release();
}

void GLWidget::resizeGL(int w, int h) {
  m_projection.setToIdentity();
  m_projection.perspective(30.0f, w / float(h), 1.0f, 1000.0f);
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
