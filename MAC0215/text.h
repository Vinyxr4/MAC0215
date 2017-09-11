#ifndef TEXT_H
#define TEXT_H

#include <QString>
#include <QDebug>
#include <QVector3D>
#include <QVector2D>
#include <vector>
#include <cmath>
#include <QImage>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "glyph.h"
#include "pdf_extractor.h"
#include "distance_transform.h"

class text {
public:
    text(QString font, QString atlas);

    void define_font_type (QString font);
    void define_atlas (QString atlas);
    void define_text (QString t, std::vector<QVector3D> quad_vertices);
    void bake_atlas ();
    void bake_mip_atlas (int max_resolution, int max_size, int layers);
    void bake_dist_transf ();
    void define_text_from_pdf (QString pdf_path);
    void gen_test ();
    void gen_test_pdf ();
    void set_layer (int l);

    std::vector<QVector3D> font_vertices;
    std::vector<QVector2D> font_texture;

private:
    QString font_path;
    QString atlas_path;
    QString text_to_render;
    FT_Library ft;
    FT_Face face;
    int layer = 0;

    std::vector<std::vector<glyph>> glyph_set;

    float dist (std::vector<std::vector<int>> img, int row, int col);
    std::vector<int> closest (std::vector<std::vector<int>> img, std::vector<std::vector<bool>> &visited, int row, int col);
};

#endif // TEXT_H
