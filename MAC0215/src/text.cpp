#include "text.h"
#include <iostream>
#include <locale>
#include <QDebug>

int dpi_test = 500;
int resol_test = 1000;

// Constructor
text::text (QString font, QString atlas) {
    define_font_type (QString (font));
    define_atlas (atlas);
    set_layer (1);
}

/*** Public metthods ***/

void text::bake_atlas() {
    bake_type = "trivial";
    bake (dpi_test, resol_test);
}

void text::bake_dist_transf (QString metric) {
    bake_type = "distance transform " + metric;
    bake (dpi_test, resol_test);
}

void text::bake_mip_atlas (int max_resolution, int max_size, int layers) {
    set_layer (layers);
    bake_type = "mip map";
    bake (max_resolution, max_size);
}

void text::define_text (QString t, std::vector<QVector3D> quad_vertices) {
    text_to_render = QString (t);

    set_layer (0);

    font_vertices.clear();
    font_texture.clear();

    int i = 0;
    for (QChar *c = text_to_render.begin(); c != text_to_render.end(); ++c, i += 4) {
        glyph g = glyph_set[layer][c->unicode()];

        float height = (quad_vertices[i+2].y()-quad_vertices[i].y())/(1.0 * g.get_height ());
        float width = (quad_vertices[i+1].x()-quad_vertices[i].x())/(1.0 * (g.get_width ()+1));
        int down_glyph = g.get_down();

        std::vector<int> indices = {0,1,2,2,1,3};
        for (int k = 0; k < (int) indices.size();++k) {
            int cur_index = i + indices[k];
            float aux1 = quad_vertices[cur_index].x();
            float aux2 = (quad_vertices[cur_index].y())-down_glyph * height;
            float aux3 = quad_vertices[cur_index].z();
            QVector3D to_push = QVector3D(aux1,aux2,aux3);
            font_vertices.push_back(to_push);
        }

        float x_offset = (1.0 * g.get_x_offset()) / x_size;
        float y_offset = (1.0 * g.get_y_offset()) / y_size;
        height = (1.0 * g.get_height ()) / y_size;
        width = (1.0 * g.get_width ()) / x_size;


        font_texture.push_back(QVector2D(x_offset, 1 - (y_offset + height)));
        font_texture.push_back(QVector2D(x_offset + width, 1 - (y_offset + height)));
        font_texture.push_back(QVector2D(x_offset, 1 - y_offset));
        font_texture.push_back(QVector2D(x_offset, 1 - y_offset));
        font_texture.push_back(QVector2D(x_offset + width, 1 - (y_offset + height)));
        font_texture.push_back(QVector2D(x_offset + width, 1 - y_offset));
    }
}

void text::define_text_curve (QString t, std::vector<QVector3D> quad_vertices) {
    text_to_render = QString (t);

    set_layer (0);

    font_vertices.clear();

    int i = 0;
    for (QChar *c = text_to_render.begin(); c != text_to_render.end(); ++c, i += 4) {
        glyph g = glyph_set[layer][c->unicode()];
        float height = (quad_vertices[i+2].y()-quad_vertices[i].y())/(1.0 * g.get_height ());
        float width = (quad_vertices[i+1].x()-quad_vertices[i].x())/(1.0 * g.get_width ());
        for (int j = 0; j < (int) g.curve_points.size(); j++) {
            QVector3D aux = QVector3D (g.curve_points[j].x()*width,g.curve_points[j].y()*height,g.curve_points[j].z());
            font_vertices.push_back(quad_vertices[i]+aux);

        }
    }
}

void text::define_text_from_pdf (QString pdf_path, QString render) {
    pdf_extractor *extractor = new pdf_extractor (pdf_path.toStdString(), code_path.toStdString());
    int offset = 0;
    int to_sum = 500;
    int num_pages;
    double *bbox;
    QString tmp_txt, txt;

    QString highlowers = "bdfhijklt";
    QString pontuation = ",.;";
    QString lower_letters = "qpg";

    extractor->init ();
    if (!read_txt) {
        num_pages = extractor->extract ();

        for (int n = 0; n < num_pages; ++n) {
            int i, j;
            tmp_txt =  QString (extractor->get_text(n).c_str());
            txt = txt + tmp_txt;
            bbox = extractor->get_bbox(n);
            for (i = 0, j = 0; i < tmp_txt.size(); ++i, j += 4) {
                float scale = 0.5;
                float low_y_scale = 1;
                float low_x_scale = 1;
                if (tmp_txt[i].isLower() && !highlowers.contains(tmp_txt[i], Qt::CaseSensitive))
                    low_y_scale = 0.7;
                if (pontuation.contains(tmp_txt[i]))
                    low_y_scale = 0.3;
                if (lower_letters.contains(tmp_txt[i]))
                    low_y_scale = 1;
                txt_vertices.push_back (QVector3D (bbox[j]+offset, bbox[j + 1], 0)*scale);
                txt_vertices.push_back (QVector3D (offset + bbox[j] +  (bbox[j + 2] - bbox[j]) * low_x_scale, bbox[j + 1], 0)*scale);
                txt_vertices.push_back (QVector3D (offset + bbox[j], bbox[j + 1] + (bbox[j + 3] - bbox[j + 1]) * low_y_scale, 0)*scale);
                txt_vertices.push_back (QVector3D (offset+ bbox[j] +  (bbox[j + 2] - bbox[j]) * low_x_scale, bbox[j + 1] + (bbox[j + 3] - bbox[j + 1]) * low_y_scale, 0)*scale);
            }
            offset += to_sum;
        }
        text_to_render = txt;
    }
    extractor->end ();
    read_txt = true;

    if (render == "curve outline")
        define_text_curve (text_to_render, txt_vertices);
    else
        define_text (text_to_render, txt_vertices);


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
}

void text::gen_test_pdf (QString render) {
    QString example_path = pdf_test_path + "Lorem-Ipsum.pdf";
    define_text_from_pdf (example_path, render);
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
    int DPI = atlas_resolution * max_resolution;
    int padding = 12;

    std::locale loc("en_US.UTF-8");
    FT_Set_Char_Size (face, 0, GLYPH_HEIGHT, DPI, DPI);
    int num_glyphs = face->num_glyphs;

    FT_Size_Metrics_ metric = face->size->metrics;
    uint texture_height = (metric.height >> 6) * (sqrt(num_glyphs));
    uint texture_width = texture_height;
    x_size = texture_width;
    y_size = texture_height;

    if (layer > 1)
        x_size *= 1.5;

    int x, y;
    int last_x = 0, last_y = 0;



    glyph_set.clear();
    QImage texture (texture_width, texture_height, QImage::Format_RGB32);
    for (int l = 0; l < 1; ++l) {
        std::vector<glyph> set;
        x = last_x, y = last_y;
        int down = 0;
        for (int i = 0; i < num_glyphs; ++i) {
            FT_Load_Char (face, i, FT_LOAD_RENDER | FT_LOAD_TARGET_LIGHT);
            FT_Bitmap *bmp = &face->glyph->bitmap;
            font_points outline;
            std::vector<QVector3D> test;
            if (std::isprint((wchar_t) i, loc)) {
                image img;
                outline = get_curve(face->glyph->outline);

                test = create_control_points(outline);

                if (x + bmp->width >= texture_width)
                    x = 0, y += (padding + (face->size->metrics.height >> 6));

                img = construct_image (bmp);
                distance_transform transform (img, bmp->rows, bmp->width);
                do_transform (transform);
                down = (face->glyph->metrics.height - face->glyph->metrics.horiBearingY) >> 6;
                prepare_texture (transform, texture, x, y);
                set.push_back (glyph (x, y, bmp->rows, bmp->width, down, i, test));

                x +=  padding + bmp->width;
            }
            else
                set.push_back (glyph (0, 0, 0, 0, down, i, test));
        }
        glyph_set.push_back(set);
        if ((l+1) % 2)
            last_x += texture_width;
        else
            last_y += texture_height;
        texture_width = texture_width >> 1;
        texture_height = texture_height >> 1;
        DPI = DPI >> 1;
        FT_Set_Char_Size (face, 0, GLYPH_HEIGHT, DPI, DPI);
    }

    QImage test = texture.scaled(atlas_dimension * texture.height(), atlas_dimension * texture.width(),Qt::KeepAspectRatio);
    test.save(atlas_path, Q_NULLPTR, 50);
    FT_Done_FreeType(ft);
}

font_points text::get_curve (FT_Outline_ glyph_outline) {
    font_points curve = font_points(glyph_outline);
    return curve;
}


bool is_on_point (font_points outline, int pos) {
    return outline.tags[pos]%2 == 1;
}



std::vector<QVector3D> text::create_control_points (font_points outline) {
    std::vector<QVector3D> control_points, not_valid;

    int curr_contour = 0;
    for (int i = 0; i < outline.n_contours; ++i) {

        int first = curr_contour, second = first+1, third = second+1;
        if (!is_on_point(outline, first)) {
            first = outline.contours[i], second = curr_contour, third = second+1;
        }
        for (int j = curr_contour; j <= outline.contours[i];) {
            if (second > outline.contours[i]) second = curr_contour;
            if (third > outline.contours[i]) third = curr_contour;
            if (is_on_point (outline, first)) {
                if (!is_on_point (outline, second) && is_on_point (outline, third)) {
                    QVector3D aux = QVector3D (outline.points[first].x>>6, outline.points[first].y>>6, 0);
                    control_points.push_back(aux);
                    aux = QVector3D (outline.points[second].x>>6, outline.points[second].y>>6, 0);
                    control_points.push_back(aux);
                    aux = QVector3D (outline.points[third].x>>6, outline.points[third].y>>6, 0);
                    control_points.push_back(aux);
                    j += 2;
                    first +=2, second = first+1, third=second+1;
                }
                else if (is_on_point (outline, second)) {
                    FT_Vector_ p_0 = outline.points[first];
                    FT_Vector_ p_1 = outline.points[second];

                    QVector3D aux = QVector3D (p_0.x>>6, p_0.y>>6, 0);
                    control_points.push_back(aux);
                    aux = QVector3D ((p_0.x>>6) + (p_1.x>>6), (p_0.y>>6) + (p_1.y>>6), 0) * 0.5;
                    control_points.push_back(aux);
                    aux = QVector3D (p_1.x>>6, p_1.y>>6, 0);
                    control_points.push_back(aux);
                    j += 1;
                    first +=1, second = first+1, third=second+1;
                }
                else if (!is_on_point (outline, third)) {
                    FT_Vector_ p_0 = outline.points[first];
                    FT_Vector_ p_1 = outline.points[second];
                    FT_Vector_ p_2 = outline.points[third];
                    FT_Vector_ p_3 = outline.points[third+1];
                    if (third + 1 > outline.contours[i])
                        p_3 = outline.points[curr_contour];

                    QVector3D aux = QVector3D (p_0.x>>6, p_0.y>>6, 0);
                    control_points.push_back(aux);
                    aux = QVector3D (p_1.x>>6, p_1.y>>6, 0);
                    control_points.push_back(aux);
                    aux = QVector3D ((p_1.x>>6) + (p_2.x>>6), (p_1.y>>6) + (p_2.y>>6), 0) * 0.5;
                    control_points.push_back(aux);
                    control_points.push_back(aux);
                    aux = QVector3D (p_2.x>>6, p_2.y>>6, 0);
                    control_points.push_back(aux);
                    aux = QVector3D (p_3.x>>6, p_3.y>>6, 0);
                    control_points.push_back(aux);

                    j += 3;
                    first +=3, second = first+1, third=second+1;
                }
            }
            else return not_valid;
        }
        curr_contour = outline.contours[i]+1;
    }
    return control_points;
}

std::vector<QVector3D> text::triangles_from_curve () {
    QVector3D fix_point = QVector3D (-10,0,0);

    std::vector<QVector3D> triangles;
    for (int i = 0; i < (int) font_vertices.size(); i += 3) {
        triangles.push_back(font_vertices[i]);
        triangles.push_back(fix_point);
        triangles.push_back(font_vertices[i+2]);
    }

    return triangles;
}

void text::do_transform (distance_transform &transform) {
    if (bake_type == "distance transform city_block")
        transform.city_block ();
    else if (bake_type == "distance transform chessboard")
        transform.chess_board ();
    else if (bake_type == "distance transform euclidean")
        transform.faster_euclidean ();
    else if (bake_type == "distance transform fast marching")
        transform.fast_marching ();

}

image text::construct_image (FT_Bitmap *bmp) {
    image img;
    for (uint row = 0; row < bmp->rows; ++row) {
        image_line line;
        for (uint col = 0; col < bmp->width; ++col) {
            if (bmp->buffer[row * bmp->pitch + col])
                line.push_back(1);
            else line.push_back(0);
        }
        img.push_back(line);
    }

    return img;
}

void text::prepare_texture (distance_transform transform, QImage &texture, int x_off, int y_off) {
    QRgb color;

    float max = transform.get_height ();
    if (transform.get_metric() == "euclidean")
        max = sqrt (transform.get_width()*transform.get_width() + transform.get_height ()*transform.get_width());
    if (transform.get_metric() == "city_block")
        max = transform.get_height () +  transform.get_width ();
    if (transform.get_metric() == "chessboard")
        max = transform.get_height () + transform.get_width();
    if (transform.get_metric() == "fast marching")
        max = sqrt(transform.get_height ()*transform.get_height () + transform.get_width()*transform.get_width());
    else if (transform.get_width () > max) max = transform.get_width ();


    for (int row = 0; row < transform.get_height (); ++row) {
        int color_cmp;
        float element;
        for (int col = 0; col < transform.get_width (); ++col) {
            element = transform.get_transform_element (row, col) / max;
            if (transform.get_metric() == "trivial")
                color_cmp = 255 * transform.get_transform_element (row, col);
            else
                color_cmp = 255 * (0.5 * element + 0.5);
            if (color_cmp > 255) color_cmp = 255;
            if (color_cmp < 0) color_cmp = 0;
            color = qRgb (color_cmp, color_cmp, color_cmp);
            texture.setPixel (col + x_off, row + y_off, color);
        }
    }
}

void text::define_font_type (QString font) {
    font_path = font;
}

void text::define_atlas (QString atlas) {
    atlas_path = atlas;
}

void text::set_atlas_dimension_value (float new_value) {
    atlas_dimension = new_value;
}

void text::set_atlas_resolution_value (float new_value) {
    atlas_resolution = new_value;
}
