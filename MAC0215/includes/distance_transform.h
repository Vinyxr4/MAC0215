#ifndef DISTANCE_TRANSFORM_H
#define DISTANCE_TRANSFORM_H

#include <vector>
#include <string>
#include <min_heap.h>

typedef std::vector<std::vector<float>> image;
typedef std::vector<int> line;
typedef std::vector<std::vector<float>> float_image;
typedef std::vector<float> image_line;
typedef std::vector<std::vector<bool>> image_check;
typedef std::vector<int> coordinate;

class distance_transform {
public:
    // Constructor: takes an image to be used
    distance_transform (image new_image, int new_height, int new_width);

    // Performs distance transform using chessboard metric
    void chess_board ();

    // Performs distance transform using city block metric
    void city_block ();

    // Performs distance transform using euclidean metric
    void euclidean ();

    // Performs distance transform using euclidean metric
    void faster_euclidean ();

    void fast_marching ();

    // Returns the transformation's element on the coordinate (row, col)
    int get_transform_element (int row, int col);

    // Returns the distance metric used
    std::string get_metric ();

    // Returns the height of the transform
    int get_height ();

    // Returns the width of the transform
    int get_width ();

private:
    image original_image;
    image transform, fmm_to_transform;
    std::string metric;
    int height, width;
    int connectivity = 8;

    // Labels to_transform cells by distance to the complementary pixel in raster swipe
    void closest_raster (image &to_transform, int row, int col);

    // Labls to_transform cells by distance to the complementary pixel in anti-raster swipe
    void closest_anti_raster (image &to_transform, int row, int col);

    // Finds the closest complementary pixel to the original image's coordinate (row, col)
    coordinate closest (std::vector<std::vector<bool>> &visited,  int row, int col);

    // Calculates the euclidean distance from the coordinate (row, col) to it's closest
    // complementary pixel
    float euclidean_distance (int row, int col);

    // Labels the current image by distance to the complementary pixel only for one color
    image one_color_run (image current_image);

    // Complements the binary image pixels
    image revert (image to_revert);

    // Join an image's one color transform and it's complement into a distance transform
    image join_binary_transform (image first_transform, image second_transform);

    image meijester_phase_1 (image to_transform);
    image meijester_phase_2 (image G);
    image transpose (image to_transpose);

    image fmm (image to_transform, int h, int w);
    int initialize_fmm(image to_transform, priority_queue &queue, image_check &checked);

    void update_neighbors (priority_queue &queue, heap_element curr, image_check &checked);
    void update_4 (priority_queue &queue, heap_element curr, image_check &checked);
    void update_4_more (priority_queue &queue, heap_element curr, image_check &checked);
    void fmm_distance (heap_element curr, int x, int y, priority_queue &queue, image_check &checked);
};

#endif // DISTANCE_TRANSFORM_H
