#include "glyph.h"

glyph::glyph (int x, int y, int h, int w, char c) {
    x_offset = x;
    y_offset = y;
    height = h;
    width = w;
    rep = c;
}
/*
glyph glyph::get_glyph (char c) {
    for (int i = 0; i < char_list.size() && char_list[i] != c; ++i);

    if (i < char_list.size ()) {
        return
    }
}
*/

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
