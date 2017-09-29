#include "distance_transform.h"
#include <cmath>
#include <QDebug>

// Constructor
distance_transform::distance_transform (image new_image, int new_height, int new_width) {
    original_image = image (new_image);
    transform = image (original_image);
    height = new_height;
    width = new_width;
    metric = "trivial";

    /*
    image test;
    for (int i = 0; i < 5; ++i) {
        std::vector<int> line;
        for (int j = 0; j < 5; ++j)
            line.push_back(0);
        test.push_back(line);
    }

    test[0][0] = 1;
    test[4][0] = 1;
    test[4][4] = 1;
    image test_transf = fmm (test);
    for (int i = 0; i < 5; ++i)
        qDebug () << test_transf[i];
        */
}

/*** Public methods ***/

void distance_transform::chess_board () {
    metric = "chessboard";
    connectivity = 8;

    image first_transform  = one_color_run (original_image);
    image second_transform = one_color_run (revert (original_image));

    transform = join_binary_transform (first_transform, second_transform);
}

void distance_transform::city_block() {
    metric = "city_block";
    connectivity = 4;

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

void distance_transform::faster_euclidean () {
    metric = "euclidean";

    image G = meijester_phase_1 (revert (original_image));
    image first_transform = meijester_phase_2 (G);

    G =meijester_phase_1 (original_image);
    image second_transform = meijester_phase_2 (G);

    transform =  join_binary_transform (first_transform, second_transform);
}

void distance_transform::fast_marching () {
    metric = "fast_marching";

    image first_transform = fmm (original_image);
    image second_transform = fmm (revert (original_image));

    transform = join_binary_transform(first_transform,second_transform);
}

int distance_transform::get_transform_element (int row, int col) {
    return transform[row][col];
}

std::string distance_transform::get_metric () {
    return metric;
}

int distance_transform::get_height () {
    return height;
}

int distance_transform::get_width () {
    return width;
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

    if (metric == "city_block")
        cl = to_transform[0].size () +  to_transform.size () - 2;

    for (int i = row - 1; i <= row; ++i) {
        if (i < 0 || i == (int) to_transform.size()) continue;
        for (int j = col - 1; j <= col + 1; ++j) {
            if (metric == "city_block" && i == row - 1 && j != col) continue;
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

    if (metric == "city_block")
        cl = to_transform[0].size () +  to_transform.size () - 2;

    for (int i = row + 1; i >= row; --i) {
        if (i < 0 || i == (int) to_transform.size()) continue;
        for (int j = col + 1; j >= col - 1; --j) {
            if (metric == "city_block" && i == row + 1 && j != col) continue;
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

image distance_transform::meijester_phase_1 (image to_transform) {
    int true_inf = height + width;
    image G;

    for (int i = 0; i < (int) to_transform.size (); ++i) {
        std::vector<int> g;

        if (to_transform[i][0]) g.push_back (0);
        else g.push_back (true_inf);

        for (int j = 1; j < (int) to_transform[0].size (); ++j) {
            if (to_transform[i][j]) g.push_back (0);
            else if (g[j - 1] == true_inf) g.push_back (true_inf);
            else g.push_back (1 + g[j - 1]);
        }

        for (int j = to_transform[i].size () - 2; j >= 0; --j)
            if (g[j + 1] < g[j]) {
                if (g[j + 1] == true_inf)
                    g[j] = true_inf;
                else
                    g[j] = 1 + g[j + 1];
            }
        G.push_back (g);
    }

    return G;
}

image distance_transform::meijester_phase_2 (image G) {
    image final_transform;

    for (int i = 0; i < (int) G.size (); ++i) {
        std::vector<int> line;
        for (int j = 0; j < (int) G[i].size (); ++j) {
            int f = i * i + G[0][j] * G[0][j];
            for (int k = 1; k < (int) G.size (); ++k) {
                int new_f = (i - k) * (i - k) + G[k][j] * G[k][j];
                if (new_f < f) f = new_f;
            }
            line.push_back (sqrt(f));
        }
        final_transform.push_back(line);
    }

    return final_transform;
}

image distance_transform::transpose (image to_transpose) {
    image transposed;

    for (int i = 0; i < (int) to_transpose.size (); ++i) {
        std::vector<int> line;
        for (int j = 0; j < (int) to_transpose[0].size (); ++j)
            line.push_back (to_transpose[j][i]);
        transposed.push_back (line);
    }

    return transposed;
}

image distance_transform::fmm (image to_transform) {
    image_check marked;
    priority_queue fmm_queue;
    image new_transform = to_transform;

    initialize_fmm (to_transform, new_transform, fmm_queue, marked);

    while (!fmm_queue.empty()) {
        heap_element dequed = fmm_queue.top();
        fmm_queue.pop ();
        if (!marked[dequed.x_pos][dequed.y_pos]) {
            marked[dequed.x_pos][dequed.y_pos] = true;
            new_transform[dequed.x_pos][dequed.y_pos] = dequed.distance;
            update_neighbors (fmm_queue, dequed, marked);
        }
    }

    return new_transform;
}


void distance_transform::initialize_fmm (image to_transform, image &new_transform, priority_queue &queue, image_check &checked) {
    for (int row = 0; row < (int) to_transform.size(); ++row) {
        std::vector<bool> line;
        for (int col = 0; col < (int) to_transform[0].size(); ++col)
            line.push_back(false);
        checked.push_back(line);
    }

    for (int row = 0; row < (int) to_transform.size(); ++row) {
        for (int col = 0; col < (int) to_transform[0].size(); ++col) {
            if (to_transform[row][col]) {
                checked[row][col] = true;
                new_transform[row][col] = 0;
                heap_element current = heap_element (row, col, 0);
                update_neighbors(queue, current, checked);
            }
        }
    }
}

void distance_transform::update_neighbors (priority_queue &queue, heap_element curr, image_check &checked) {
    update_4 (queue, curr, checked);
    if (connectivity == 8)
        update_4_more (queue, curr, checked);
}

void distance_transform::update_4 (priority_queue &queue, heap_element curr, image_check &checked) {
    fmm_distance (curr, curr.x_pos, curr.y_pos-1, queue, checked);
    fmm_distance (curr, curr.x_pos+1, curr.y_pos, queue, checked);
    fmm_distance (curr, curr.x_pos, curr.y_pos+1, queue, checked);
    fmm_distance (curr, curr.x_pos-1, curr.y_pos, queue, checked);
}

void distance_transform::update_4_more (priority_queue &queue, heap_element curr, image_check &checked) {
    fmm_distance (curr, curr.x_pos-1, curr.y_pos-1, queue, checked);
    fmm_distance (curr, curr.x_pos+1, curr.y_pos-1, queue, checked);
    fmm_distance (curr, curr.x_pos+1, curr.y_pos+1, queue, checked);
    fmm_distance (curr, curr.x_pos-1, curr.y_pos+1, queue, checked);
}

void distance_transform::fmm_distance (heap_element curr, int x, int y, priority_queue &queue, image_check &checked) {
    int h = checked.size(), w = checked[0].size();
    if (x >= 0 && y >= 0 && x < h && y < w && !checked[x][y]) {
        float distance = curr.distance + sqrt ((curr.x_pos-x)*(curr.x_pos-x)+(curr.y_pos-y)*(curr.y_pos-y));
        heap_element new_element = heap_element (x, y, distance);
        queue.push(new_element);
    }
}
