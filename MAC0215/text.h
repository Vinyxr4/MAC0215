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
    // Constructor: takes a font path and a path to save the atlas
    text(QString font, QString atlas);

    // Wrapper to trivially bake the atlas
    void bake_atlas ();

    // Bakes a mipmap atlas with layers, max_size, and max_resolution
    void bake_mip_atlas (int max_resolution, int max_size, int layers);

    // Wrapper to bake an atlas using distance transform
    void bake_dist_transf ();

    // Defines the text to be rendered from a pdf text
    void define_text_from_pdf (QString pdf_path);

    // Defines the text to be rendered from a Qstring t, on the quad_vertices
    void define_text (QString t, std::vector<QVector3D> quad_vertices);

    // Generates a simple test
    void gen_test ();

    // Generates a test using a pdf file
    void gen_test_pdf ();

    // Set the number of layers
    void set_layer (int l);

    std::vector<QVector3D> font_vertices;
    std::vector<QVector2D> font_texture;

private:
    QString font_path, atlas_path, text_to_render;
    FT_Library ft;
    FT_Face face;
    int layer = 1;
    QString bake_type = "trivial";
    uint x_size, y_size;
    std::vector<std::vector<glyph>> glyph_set;
    QString code_path = "../MAC0215/";

    // Bakes an atlas
    void bake (int max_resolution, int max_size);

    // Defines the font to be used
    void define_font_type (QString font);

    // Defines the path used to save the baked atlas
    void define_atlas (QString atlas);
};

#endif // TEXT_H
