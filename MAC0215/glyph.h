#ifndef GLYPH_H
#define GLYPH_H

#include <QString>
#include <QDebug>
#include <QImage>
#include <ft2build.h>
#include FT_FREETYPE_H


class glyph {
public:
    glyph (int x, int y, int h, int w, char c);
    char get_char ();
    int get_x_offset ();
    int get_y_offset ();
    int get_height ();
    int get_width ();

private:
    int height, width;
    int x_offset, y_offset;
    char rep;
};

#endif // GLYPH_H
