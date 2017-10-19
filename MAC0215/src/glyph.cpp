#include "glyph.h"

glyph::glyph (int x, int y, int h, int w, int d, char c, std::vector<QVector3D> c_points) {
    x_offset = x;
    y_offset = y;
    height = h;
    width = w;
    rep = c;

        down = d;
    curve_points = std::vector<QVector3D> (c_points);
}

char glyph::get_char () {
    return rep;
}

int glyph::get_x_offset () {
    return x_offset;
}

int glyph::get_y_offset () {
    return y_offset;
}

int glyph::get_height () {
    return height;
}

int glyph::get_width () {
    return width;
}

int glyph::get_down () {
    return down;
}
