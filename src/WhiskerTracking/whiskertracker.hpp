#ifndef WHISKERTRACKER_HPP
#define WHISKERTRACKER_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <map>

#include "JaneliaWhiskerTracker/janelia.hpp"
#include "whisker.hpp"
#include "Geometry/mask.hpp"

namespace whisker {

class WhiskerTracker {

public:
    WhiskerTracker();

    void trace(const std::vector<uint8_t> &image, const int image_height, const int image_width);

    std::tuple<float, int> get_nearest_whisker(float x_p, float y_p);

    float getWhiskerLengthThreshold() const { return _whisker_length_threshold; };

    void setWhiskerLengthThreshold(float length_threshold) { _whisker_length_threshold = length_threshold; };

    float getWhiskerPadRadius() const { return _whisker_pad_radius; };

    void setWhiskerPadRadius(float whisker_pad_radius) { _whisker_pad_radius = whisker_pad_radius; };

    Point2D<float> getWhiskerPad() const { return _whisker_pad; };

    void setWhiskerPad(float w_x, float w_y) { _whisker_pad = Point2D<float>{w_x, w_y}; };

    void setFaceMask(std::vector<Point2D<float>> mask)
    {
        _face_mask = create_mask( mask);
        _face_mask_set = whisker::create_set(_face_mask);
    };

    void setImageHeight(int const height) {_image_height = height;};

    void setImageWidth(int const width) {_image_width = width;};

    void setHeadDirection(float x, float y);

    enum JaneliaParameter {
        SEED_ON_GRID_LATTICE_SPACING,
        SEED_SIZE_PX,
        SEED_ITERATIONS,
        SEED_ITERATION_THRESH,
        SEED_ACCUM_THRESH,
        SEED_THRESH,
        HAT_RADIUS,
        MIN_LEVEL,
        MIN_SIZE,
        TLEN,
        OFFSET_STEP,
        ANGLE_STEP,
        WIDTH_STEP,
        WIDTH_MIN,
        WIDTH_MAX,
        MIN_SIGNAL,
        MAX_DELTA_ANGLE,
        MAX_DELTA_WIDTH,
        MAX_DELTA_OFFSET,
        HALF_SPACE_ASSYMETRY_THRESH,
        HALF_SPACE_TUNNELING_MAX_MOVES
    };

    void changeJaneliaParameter(JaneliaParameter parameter, float value);

    std::vector<Line2D> whiskers;

private:
    janelia::JaneliaTracker _janelia;
    bool _janelia_init {false};
    float _whisker_length_threshold {75.0};
    float _whisker_pad_radius {150.0f};
    Point2D<float> _whisker_pad {0.0f, 0.0f};
    Mask2D _face_mask;
    std::set<Point2D<int>> _face_mask_set;
    int _image_height {480};
    int _image_width {640};
    GeomVector _head_direction_vector {0.0, 1.0};
    bool _verbose {false};

    void _removeWhiskersByWhiskerPadRadius();

    void _reinitializeJanelia();

    //void _clipFaceMask();
    void _connectToFaceMask();

    void _orderWhiskers();

};

void _alignWhiskerToFollicle(Line2D &whisker, whisker::Point2D<float> whisker_pad);
void _eraseWhiskers(std::vector<Line2D> & whiskers, std::vector<int> &erase_inds);
void _removeDuplicates(std::vector<Line2D> & whiskers);

std::map<int, std::vector<Line2D>> load_janelia_whiskers(std::string const & filename);

}


#endif // WHISKERTRACKER_HPP
