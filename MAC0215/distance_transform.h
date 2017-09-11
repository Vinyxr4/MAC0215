#ifndef DISTANCE_TRANSFORM_H
#define DISTANCE_TRANSFORM_H

#include <vector>

typedef std::vector<std::vector<int>> image;
typedef std::vector<int> coordinate;

class distance_transform {
public:
    distance_transform (image new_image);
    void chess_board ();
    void euclidean ();
    int get_transform_element (int row, int col);

private:
    image original_image;
    image transform;

    void closest_raster (image &to_transform, int row, int col);
    void closest_anti_raster (image &to_transform, int row, int col);
    coordinate closest (std::vector<std::vector<bool>> &visited,  int row, int col);
    float euclidean_distance (int row, int col);
    image one_color_run (image current_image);
    image revert (image to_revert);
    image join_binary_transform (image first_transform, image second_transform);
};

#endif // DISTANCE_TRANSFORM_H
