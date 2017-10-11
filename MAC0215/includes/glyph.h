#ifndef GLYPH_H
#define GLYPH_H

#include <vector>
#include <ft2build.h>
#include <QVector3D>
#include FT_FREETYPE_H

// This class represents a glyph, with x and y offsets on it's
// containing atlas,wich char it represents and height and width.

typedef FT_Outline font_points;

class glyph {
public:
    // Glyph constructor: recieves lower left position, height, width and char representation
    glyph (int x, int y, int h, int w, char c, std::vector<QVector3D> c_points);

    // Returns char represented by the glyph
    char get_char ();

    // Returns lower left x coordinate of the glyph on the atlas
    int get_x_offset ();

    // Returns lower left y coordinate of the glyph on the atlas
    int get_y_offset ();

    // Returns glyph height
    int get_height ();

    // Returns glyph width
    int get_width ();

    std::vector<QVector3D> curve_points;

private:
    int height, width, x_offset, y_offset;
    char rep;
};

#endif // GLYPH_H
