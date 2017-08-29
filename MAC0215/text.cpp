#include "text.h"

text::text (QString font) {
    define_font_type (QString (font));
    bake_atlas ();

    //std::vector<QVector3D> vec;
    //vec.push_back(QVector3D (0,0,0));
    //vec.push_back(QVector3D (0,0,0));
    //vec.push_back(QVector3D (0,0,0));
    //vec.push_back(QVector3D (0,0,0));

    //define_text(QString ("p"),vec);
    //qDebug () << font_texture;
}


void text::bake_atlas() {
    if (FT_Init_FreeType (&ft))
        qDebug() << "ruim init";
    if(FT_New_Face(ft, font_path.toStdString().c_str(), 0, &face))
        qDebug() << "ruim new face";

    int GLYPH_HEIGHT = 1000;
    int DPI = 250;

    FT_Set_Char_Size(face, 0, GLYPH_HEIGHT, DPI, DPI);

    int num_glyphs = face->num_glyphs;

    uint texture_height = (face->size->metrics.height >> 6) * (sqrt(num_glyphs));
    uint texture_width = texture_height;

    QImage texture(texture_width, texture_height, QImage::Format_RGB32);
    QRgb color;
    int x = 0;
    int y = 0;

    for (int i = 0; i < num_glyphs; ++i) {
        FT_Load_Char (face, i, FT_LOAD_RENDER| FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
        FT_Bitmap *bmp = &face->glyph->bitmap;

        if (x + bmp->width >= texture_width)
            x = 0, y += (1 + (face->size->metrics.height >> 6));

        for (uint row = 0; row < bmp->rows; ++row) {
            for (uint col = 0; col < bmp->width; ++col) {
                int color_cmp = bmp->buffer[row * bmp->pitch + col] * (int) (255.0/80.0);
                color = qRgb (color_cmp,color_cmp,color_cmp);
                texture.setPixel(col + x, row + y, color);
            }
        }

        glyph *g = new glyph (x, y, bmp->rows, bmp->width, i);
        glyph_set.push_back (*g);

        x += bmp->width;
    }

    texture.save("atlas.png", Q_NULLPTR, 50);

    FT_Done_FreeType(ft);
}

void text::define_text (QString t, std::vector<QVector3D> quad_vertices) {
    text_to_render = QString (t);

    int i = 0;
    for (QChar *c = text_to_render.begin(); c != text_to_render.end(); ++c, ++i) {
        font_vertices.push_back(quad_vertices[i]);
        font_vertices.push_back(quad_vertices[i + 1]);
        font_vertices.push_back(quad_vertices[i + 2]);
        font_vertices.push_back(quad_vertices[i + 2]);
        font_vertices.push_back(quad_vertices[i + 1]);
        font_vertices.push_back(quad_vertices[i + 3]);

        int ch = c->unicode();

        glyph g = glyph_set[ch];


        int x_offset = g.get_x_offset();
        int y_offset = g.get_y_offset();
        int height = g.get_height ();
        int width = g.get_width ();

        font_texture.push_back(QVector2D(x_offset, y_offset + height));
        font_texture.push_back(QVector2D(x_offset + width, y_offset + height));
        font_texture.push_back(QVector2D(x_offset, y_offset));
        font_texture.push_back(QVector2D(x_offset, y_offset));
        font_texture.push_back(QVector2D(x_offset + width, y_offset + height));
        font_texture.push_back(QVector2D(x_offset + width, y_offset));
    }
}

void text::define_font_type (QString font) {
    font_path = font;
}
