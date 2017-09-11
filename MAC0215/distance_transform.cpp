#include "distance_transform.h"
#include <cmath>

// Constructor
distance_transform::distance_transform (image new_image) {
    original_image = image (new_image);
    transform = image (original_image);
}

/*** Public methods ***/

void distance_transform::chess_board () {
    image first_transform  = one_color_run (original_image);
    image second_transform = one_color_run (revert (original_image));

    transform = join_binary_transform (first_transform, second_transform);
}

void distance_transform::euclidean () {
    transform = image (original_image);

    for (int i = 0; i < (int) transform.size (); ++i)
        for (int j = 0; j < (int) transform[0].size (); ++j)
            transform[i][j] = euclidean_distance (i, j);
}

int distance_transform::get_transform_element (int row, int col) {
    return transform[row][col];
}

/*** Private methods ***/

image distance_transform::revert (image to_revert) {
    image reverted_image (to_revert);

    for (int i = 0; i < (int) reverted_image.size (); ++i)
        for (int j = 0; j < (int) reverted_image[0].size (); ++j)
            reverted_image[i][j] = (reverted_image[i][j] + 1) % 2;

    return reverted_image;
}

image distance_transform::one_color_run (image current_image) {
    image aux_image = image (current_image);

    for (int i = 0; i < (int) aux_image.size (); ++i)
        for (int j = 0; j < (int) aux_image[0].size (); ++j)
            if (aux_image[i][j])
                closest_raster (aux_image, i, j);

    for (int i = aux_image.size() - 1; i >= 0; --i)
        for (int j = aux_image[0].size() - 1; j >= 0; --j)
            if (aux_image[i][j])
                closest_anti_raster (aux_image, i, j);

    return aux_image;
}

image distance_transform::join_binary_transform (image first_transform, image second_transform) {
    image result = image (first_transform);

    for (int i = 0; i < (int) result.size (); ++i)
        for (int j = 0; j < (int) result[0].size (); ++j)
            if (!result[i][j])
                result[i][j] = -second_transform[i][j];

    return result;
}

void distance_transform::closest_raster (image &to_transform, int row, int col) {
    int cl = to_transform[0].size ();
    if ((int) to_transform.size () > cl)
        cl = to_transform.size ();

    for (int i = row - 1; i <= row; ++i) {
        if (i < 0 || i == (int) to_transform.size()) continue;
        for (int j = col - 1; j <= col + 1; ++j) {
            if (j < 0 || j == (int) to_transform[0].size()) continue;
            if (i == row && j == col) break;
            if (to_transform[i][j] < cl)
                cl = to_transform[i][j];
        }
    }
    to_transform[row][col] = 1 + cl;
}

void distance_transform::closest_anti_raster (image &to_transform, int row, int col) {
    int cl = to_transform[0].size ();
    if ((int) to_transform.size () > cl)
        cl = to_transform.size ();

    for (int i = row + 1; i >= row; --i) {
        if (i < 0 || i == (int) to_transform.size()) continue;
        for (int j = col + 1; j >= col - 1; --j) {
            if (j < 0 || j == (int) to_transform[0].size()) continue;
            if (i == row && j == col) break;
            if (to_transform[i][j] < cl)
                cl = to_transform[i][j];
        }
    }

    if (1 + cl < to_transform[row][col])
        to_transform[row][col] = 1 + cl;
}

float distance_transform::euclidean_distance (int row, int col) {
    std::vector<std::vector<bool>> visited;

    for (int i = 0; i < (int) original_image.size(); ++i) {
        std::vector<bool> visit_line;
        for (int j = 0; j < (int) original_image[0].size(); ++j)
            visit_line.push_back(false);
        visited.push_back(visit_line);
    }

    coordinate pair = closest (visited, row, col);
    float d = sqrt ((pair[0] - row) * (pair[0] - row) + (pair[1] - col) * (pair[1] - col));
    if (!original_image[row][col])
        d *= -1;

    return d;
}

coordinate distance_transform::closest (std::vector<std::vector<bool>> &visited,  int row, int col) {
    coordinate pair (2);
    pair[0] = row;
    pair[1] = col;

    visited[row][col] = true;
    for (int i = row - 1; i <= row + 1; ++i) {
        if (i < 0 ||  i >= (int) original_image.size()) continue;
        for (int j = col - 1; j <= col + 1; ++j) {
            if (j < 0 || j >= (int) original_image[0].size () || visited[i][j]) continue;
            if (original_image[i][j] != original_image[row][col]) {
                pair[0] = i, pair[1] = j;
                return pair;
            }
            else
                pair = closest (visited, i, j);
        }
    }

    return pair;
}
