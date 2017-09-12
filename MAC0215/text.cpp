#include "text.h"
#include <iostream>
#include <locale>

// Constructor
text::text (QString font, QString atlas) {
    define_font_type (QString (font));
    define_atlas(atlas);
}

/*** Public metthods ***/

void text::bake_atlas() {
    bake_type = "trivial";
    bake (500, 1000);
}

void text::bake_dist_transf () {
    bake_type = "distance transform";
    bake (500, 1000);
}

void text::bake_mip_atlas (int max_resolution, int max_size, int layers) {
    set_layer (layers);
    bake_type = "mip map";
    bake (max_resolution, max_size);
}

void text::define_text (QString t, std::vector<QVector3D> quad_vertices) {
    text_to_render = QString (t);

    set_layer (0);

    int i = 0;
    for (QChar *c = text_to_render.begin(); c != text_to_render.end(); ++c, i += 4) {
        font_vertices.push_back(quad_vertices[i]);
        font_vertices.push_back(quad_vertices[i + 1]);
        font_vertices.push_back(quad_vertices[i + 2]);
        font_vertices.push_back(quad_vertices[i + 2]);
        font_vertices.push_back(quad_vertices[i + 1]);
        font_vertices.push_back(quad_vertices[i + 3]);

        glyph g = glyph_set[layer][c->unicode()];

        float x_offset = (1.0 * g.get_x_offset()) / x_size;
        float y_offset = (1.0 * g.get_y_offset()) / y_size;
        float height = (1.0 * g.get_height ()) / y_size;
        float width = (1.0 * g.get_width ()) / x_size;

        font_texture.push_back(QVector2D(x_offset, 1 - (y_offset + height)));
        font_texture.push_back(QVector2D(x_offset + width, 1 - (y_offset + height)));
        font_texture.push_back(QVector2D(x_offset, 1 - y_offset));
        font_texture.push_back(QVector2D(x_offset, 1 - y_offset));
        font_texture.push_back(QVector2D(x_offset + width, 1 - (y_offset + height)));
        font_texture.push_back(QVector2D(x_offset + width, 1 - y_offset));
    }
}

void text::define_text_from_pdf (QString pdf_path) {
    pdf_extractor *extractor = new pdf_extractor (pdf_path.toStdString(), code_path.toStdString());
    QString txt;
    double *bbox;
    int num_pages;

    extractor->init ();
    num_pages = extractor->extract ();
    txt = QString (extractor->get_text(0).c_str());
    bbox = extractor->get_bbox(0);
    extractor->end ();

    std::vector<QVector3D> txt_vertices;
    int i, j;
    float scale = 0.5;
    for (i = 0, j = 0; i < txt.size(); ++i, j += 4) {
        txt_vertices.push_back (QVector3D (bbox[j], bbox[j + 1], 0)*scale);
        txt_vertices.push_back (QVector3D (bbox[j + 2], bbox[j + 1], 0)*scale);
        txt_vertices.push_back (QVector3D (bbox[j], bbox[j + 3], 0)*scale);
        txt_vertices.push_back (QVector3D (bbox[j + 2], bbox[j + 3], 0)*scale);
    }

    define_text (txt, txt_vertices);
}

void text::set_layer (int l) {
    layer = l;
}

void text::gen_test () {
    QString test_string ("Olá!");

    std::vector<QVector3D> vec;
    vec.push_back(QVector3D (-2.0, 0, 0));
    vec.push_back(QVector3D (-1.5, 0, 0));
    vec.push_back(QVector3D (-2.0, 0.5, 0));
    vec.push_back(QVector3D (-1.5, 0.5, 0));

    vec.push_back(QVector3D (-1.5, 0, 0));
    vec.push_back(QVector3D (-1.0, 0, 0));
    vec.push_back(QVector3D (-1.5, 0.5, 0));
    vec.push_back(QVector3D (-1.0, 0.5, 0));

    vec.push_back(QVector3D (-1.0, 0, 0));
    vec.push_back(QVector3D (-0.5, 0, 0));
    vec.push_back(QVector3D (-1.0, 0.5, 0));
    vec.push_back(QVector3D (-0.5, 0.5, 0));

    vec.push_back(QVector3D (-0.5, 0, 0));
    vec.push_back(QVector3D (-0.3, 0, 0));
    vec.push_back(QVector3D (-0.5, 0.5, 0));
    vec.push_back(QVector3D (-0.3, 0.5, 0));

    define_text(test_string, vec);

    //qDebug () << "Coordenadas:\n" << font_texture;
}

void text::gen_test_pdf () {
    QString example_path = code_path + "pdf/lista_1.pdf";
    define_text_from_pdf (example_path);
}

/*** Private methods ***/

void text::bake (int max_resolution, int max_size) {
    if (max_resolution > 500 || max_size > 1000) {
        qDebug () << "Too high!\n";
        return;
    }
    if (FT_Init_FreeType (&ft))
        qDebug() << "ruim init";
    if(FT_New_Face(ft, font_path.toStdString().c_str(), 0, &face))
        qDebug() << "ruim new face";

    int GLYPH_HEIGHT = max_size;
    int DPI = max_resolution;

    FT_Select_Charmap(face , ft_encoding_unicode);
    FT_Set_Char_Size(face, 0, GLYPH_HEIGHT, DPI, DPI);
    int num_glyphs = face->num_glyphs;

    FT_Size_Metrics_ metric = face->size->metrics;

    uint texture_height = (metric.height >> 6) * (sqrt(num_glyphs));
    uint texture_width = texture_height;
    x_size = texture_width;
    y_size = texture_height;
    if (layer > 1)
        x_size *= 1.5;

    QImage texture(texture_width, texture_height, QImage::Format_RGB32);
    QRgb color;
    int x = 0;
    int y = 0;
    int last_x = 0;
    int last_y = 0;

    std::locale loc("en_US.UTF-8");
    for (int l = 0; l < layer; ++l) {
        std::vector<glyph> set;
        for (int i = 0; i < num_glyphs; ++i) {
            if (std::isprint((wchar_t) i, loc)) {
                std::vector<std::vector<int>> img;

                FT_Load_Char (face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
                FT_Bitmap *bmp = &face->glyph->bitmap;

                if (x + bmp->width >= texture_width)
                    x = 0, y += (1 + (face->size->metrics.height >> 6));

                for (uint row = 0; row < bmp->rows; ++row) {
                    std::vector<int> line;
                    for (uint col = 0; col < bmp->width; ++col) {
                        if (bmp->buffer[row * bmp->pitch + col])
                            line.push_back(1);
                        else line.push_back(0);
                    }
                    img.push_back(line);
                }

                distance_transform transform (img);
                if (bake_type == "distance transform")
                    transform.chess_board ();

                for (uint row = 0; row < img.size(); ++row) {
                    for (uint col = 0; col < img[0].size(); ++col) {
                        float max = img[0].size();
                        if (img[0].size () > max) max = img[0].size ();
                        int element = transform.get_transform_element (row, col);
                        int color_cmp = (int) (255 * (0.5 * element/max + 0.5));
                        color = qRgb (color_cmp,color_cmp,color_cmp);
                        texture.setPixel(col + x, row + y, color);
                    }
                }

                glyph *g = new glyph (x, y, bmp->rows, bmp->width, i);
                set.push_back (*g);
                x +=  1 + bmp->width;
            }
            else {
                glyph *g = new glyph (0, 0, 0, 0, i);
                set.push_back (*g);
            }
        }
        glyph_set.push_back(set);
        if ((l+1) % 2)
            last_x += texture_width;
        else
            last_y += texture_height;
        x = last_x;
        y = last_y;
        texture_width = texture_width >> 1;
        texture_height = texture_height >> 1;
        DPI = DPI >> 1;
        FT_Set_Char_Size(face, 0, GLYPH_HEIGHT, DPI, DPI);
    }

    texture.save(atlas_path, Q_NULLPTR, 50);
    FT_Done_FreeType(ft);
}

void text::define_font_type (QString font) {
    font_path = font;
}

void text::define_atlas (QString atlas) {
    atlas_path = atlas;
}
