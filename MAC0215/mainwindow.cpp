#include "mainwindow.h"
#include "window.h"

#include <QMenu>
#include <QDebug>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QVector3D>
#include <vector>

MainWindow::MainWindow() {
  QMenuBar *menuBar = new QMenuBar;
  QMenu *menuWindow = menuBar->addMenu(tr("&Open"));
  QAction *openAction = new QAction(menuWindow);
  openAction->setText(tr("Open OBJ..."));
  connect(openAction, &QAction::triggered, this, &MainWindow::loadFile);

  QAction *exitAction = new QAction(menuWindow);
  exitAction->setText(tr("Exit"));
  connect(exitAction, &QAction::triggered, this, &MainWindow::exit);

  menuWindow->addAction(openAction);
  menuWindow->addSeparator();
  menuWindow->addAction(exitAction);
  setMenuBar(menuBar);

  window_ = new Window(this, 100, 64);
  setCentralWidget(window_);
  setWindowTitle("PA2");
}

void MainWindow::loadFile() {
  // We need to disconnect the widget update to load a QFileDialog, as the
  // OpenGL draw function interrupt the dialog rendering
  window_->disconnectWidgetUpdate();
  QString file = QFileDialog::getOpenFileName(this);

  QFile obj (file);
  obj.open(QIODevice::ReadOnly);
  QTextStream obj_stream (&obj);

  std::vector<QVector3D> verts, vertsNorm;
  std::vector<uint> inds, indsNorm;
  QString line;
  bool has_norm = false;
  while (!obj_stream.atEnd()) {
    line = obj_stream.readLine(200);
    if (line.indexOf(QString("vn")) > -1) {
        QStringList list = line.split(" ");
        has_norm = true;
        vertsNorm.push_back (QVector3D (list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
    }
    else if (line[0] == 'v') {
        QStringList list = line.split(" ");
        verts.push_back (QVector3D (list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
    }
    else if (line[0] == 'f') {
        QStringList list = line.split(QRegExp("\\s+|/[0-9]*/"));
        if (!has_norm) {
            for (int j = 1; j < 4; ++j)
                inds.push_back (list[j].toUInt() - 1);
        }
        else {
            for (int j = 1; j < 7; ++j) {
                inds.push_back (list[j++].toUInt() - 1);
                indsNorm.push_back (list[j].toUInt() - 1);
            }
        }
    }
  }

  std::vector<QVector3D> faceNorms;
  if (!has_norm) {
    for (int i = 0; i < (int) inds.size(); i += 3) {
        QVector3D u(verts[inds[i]] - verts[inds[i+2]]);
        QVector3D v(verts[inds[i+1]] - verts[inds[i]]);
        QVector3D prod = u.crossProduct(u,v);
        faceNorms.push_back(prod);
    }
    for (int i = 0; i < (int) verts.size();++i) {
        vertsNorm.push_back(QVector3D(0.0f, 0.0f, 0.0f));
        int k = 0;
        for (int j = 0; j < (int) faceNorms.size(); ++j, k += 3)
            if (inds[k] == (uint) i || inds[k+1] == (uint) i || inds[k+2] == (uint) i)
                vertsNorm[i] += faceNorms[j];
    }
  }

  QVector3D *vert = (QVector3D*) malloc (verts.size () * sizeof (QVector3D));
  QVector3D *norm = (QVector3D*) malloc (verts.size () * sizeof (QVector3D));
  uint *ind = (uint*) malloc (inds.size () * sizeof (uint));
  uint *indNorm = (uint*) malloc (inds.size () * sizeof (uint));

  for (int i = 0; i < (int)verts.size(); ++i)
     vert[i] = verts[i];

  for (int i = 0; i < (int)inds.size(); ++i) {
     ind[i] = inds[i];
      if (!has_norm)
         indNorm[i] = inds[i];
     else
         indNorm[i] = indsNorm[i];
  }

  if (!has_norm)
    for (int i = 0; i < (int)vertsNorm.size(); ++i)
         norm[i] = vertsNorm[i].normalized();
  else
    for (int i = 0; i < (int)inds.size(); ++i)
        norm[inds[i]] = vertsNorm[indsNorm[i]].normalized();

  obj.close();

  window_->load(inds.size(), verts.size (), ind, vert, norm);

  window_->connectWidgetUpdate();

  free(norm);
  free (vert);
  free (ind);
  free (indNorm);
}

void MainWindow::exit() {
  QApplication::exit();
}
