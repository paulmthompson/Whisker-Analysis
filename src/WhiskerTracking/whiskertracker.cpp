#include "whiskertracker.hpp"

#include "JaneliaWhiskerTracker/io.hpp"

#include <omp.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>

janelia::JaneliaTracker whisker::WhiskerTracker::_janelia;
bool whisker::WhiskerTracker::_janelia_init;

namespace whisker {

janelia::Image<uint8_t> bg = janelia::Image<uint8_t>(640, 480, std::vector<uint8_t>(640 * 480, 0));

WhiskerTracker::WhiskerTracker()
{
    _janelia = janelia::JaneliaTracker();
    _janelia_init = false;
}

std::vector<std::vector<Line2D>> WhiskerTracker::trace_multiple_images(const std::vector<std::vector<uint8_t>> & images, const int image_height, const int image_width) {

    std::vector<std::vector<Line2D>> whiskers(images.size());

    #ifndef _MSC_VER
    #pragma omp parallel
    {
        _reinitializeJanelia();
    }
    #endif

    #ifndef _MSC_VER
    #pragma omp parallel for
    #endif
    for (int i = 0; i < static_cast<int>(images.size()); i++) {
        whiskers[i] = trace(images[i], image_height, image_width);
    }

    return whiskers;
}

std::vector<Line2D> WhiskerTracker::trace(const std::vector<uint8_t> & image, const int image_height, const int image_width) {

    _reinitializeJanelia();

    std::vector<Line2D> whiskers{};

    auto t0 = std::chrono::high_resolution_clock::now();

    auto img = janelia::Image<uint8_t>(image_width, image_height, image);

    auto t1 = std::chrono::high_resolution_clock::now();

    auto j_segs = _janelia.find_segments(1, img, bg);

    auto t2 = std::chrono::high_resolution_clock::now();

    for (auto &w_seg: j_segs) {
        auto whisker = create_line(w_seg.x, w_seg.y);
        if (length(whisker) > _whisker_length_threshold) {
            whiskers.push_back(std::move(whisker));
        }
    }

    auto t3 = std::chrono::high_resolution_clock::now();

    remove_duplicates(whiskers);
    std::ranges::for_each(whiskers, [wp=_whisker_pad](Line2D & w)
    {align_whisker_to_follicle(w, wp);});

    auto t4 = std::chrono::high_resolution_clock::now();

    _connectToFaceMask(whiskers);

    auto t5 = std::chrono::high_resolution_clock::now();

    _removeWhiskersByWhiskerPadRadius(whiskers);

    auto t6 = std::chrono::high_resolution_clock::now();

    order_whiskers(whiskers, _head_direction_vector);

    auto t7 = std::chrono::high_resolution_clock::now();

    if (_verbose) {
        std::cout << "Image conversion: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << "ms" << std::endl;
        std::cout << "Janelia find segments: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
        std::cout << "Create whiskers: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << "ms" << std::endl;
        std::cout << "Remove duplicates: " << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count() << "ms" << std::endl;
        std::cout << "Connect to face mask: " << std::chrono::duration_cast<std::chrono::milliseconds>(t5 - t4).count() << "ms" << std::endl;
        std::cout << "Remove whiskers by whisker pad radius: " << std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count() << "ms" << std::endl;
        std::cout << "Order whiskers: " << std::chrono::duration_cast<std::chrono::milliseconds>(t7 - t6).count() << "ms" << std::endl;
    }

    return whiskers;
}

std::tuple<float, int> get_nearest_whisker(std::vector<Line2D> & whiskers, float x_p, float y_p) {

    float nearest_distance = 1000.0;
    int whisker_id = 0;

    float current_d = 0.0f;
    int current_whisker_id = 0;

    for (auto &w: whiskers) {
        for (int i = 0; i < w.size(); i++) {
            current_d = sqrt(pow(x_p - w[i].x, 2) + pow(y_p - w[i].y, 2));
            if (current_d < nearest_distance) {
                nearest_distance = current_d;
                whisker_id = current_whisker_id;
            }
        }
        current_whisker_id += 1;
    }

    return std::make_tuple(nearest_distance, whisker_id);
}

std::map<int, std::vector<Line2D>> load_janelia_whiskers(std::string const & filename) {
    auto j_segs = janelia::load_binary_data(filename);

    auto output_whiskers = std::map<int, std::vector<Line2D>>();

    for (auto const & w_seg: j_segs) {

        if (!output_whiskers.contains(w_seg.time)) { // Key doesn't exist
            output_whiskers[w_seg.time] = std::vector<Line2D>();
        }

        output_whiskers[w_seg.time].push_back(create_line(w_seg.x, w_seg.y));

    }

    return output_whiskers;
}

void WhiskerTracker::setHeadDirection(float x, float y)
{
    _head_direction_vector = GeomVector{x,y};
    _head_direction_vector = normalize(_head_direction_vector);
}

/**
 * @brief align_whisker_to_follicle
 *
 * Measures the distance between the Point at one end of a whisker and Point
 * at the other end. The whisker is then flipped so that the first index is closest
 * to the follicle
 *
 *
 * @param whisker whisker to be checked
 */
void align_whisker_to_follicle(Line2D & whisker, whisker::Point2D<float> whisker_pad) {

    auto start_distance = distance(whisker[0], whisker_pad);

    auto end_distance = distance(whisker.back(), whisker_pad);

    if (start_distance > end_distance) {
        std::ranges::reverse(whisker);
    }
}

void WhiskerTracker::changeJaneliaParameter(JaneliaParameter parameter, float value) {
    switch (parameter) {
        case SEED_ON_GRID_LATTICE_SPACING: {
            _janelia.config._lattice_spacing = static_cast<int>(value);
            break;
        }
        case SEED_SIZE_PX: {
            _janelia.config._maxr = static_cast<int>(value);
            break;
        }
        case SEED_ITERATIONS: {
            _janelia.config._maxiter = static_cast<int>(value);
            break;
        }
        case SEED_ITERATION_THRESH: {
            _janelia.config._iteration_thres = value;
            break;
        }
        case SEED_ACCUM_THRESH: {
            _janelia.config._accum_thres = value;
            break;
        }
        case SEED_THRESH: {
            _janelia.config._accum_thres = value;
            break;
        }
        case HAT_RADIUS: {

        }
        case MIN_LEVEL: {

        }
        case MIN_SIZE: {

        }
        case TLEN: {
            _janelia.config._tlen = value;
            _reinitializeJanelia();
            break;
        }
        case OFFSET_STEP: {
            _janelia.config._offset_step = value;
            _reinitializeJanelia();
            break;
        }
        case ANGLE_STEP: {
            _janelia.config._angle_step = value;
            _reinitializeJanelia();
            break;
        }
        case WIDTH_STEP: {
            _janelia.config._width_step = value;
            _reinitializeJanelia();
        }
        case WIDTH_MIN: {
            // Must be multiple of width step
            _janelia.config._width_min = value;
            _reinitializeJanelia();
            break;
        }
        case WIDTH_MAX: {
            _janelia.config._width_max = value;
            _reinitializeJanelia();
            break;
        }
        case MIN_SIGNAL: {
            _janelia.config._min_signal = value;
            _reinitializeJanelia();
            break;
        }
        case MAX_DELTA_ANGLE: {
            _janelia.config._max_delta_angle = value;
            _reinitializeJanelia();
            break;
        }
        case MAX_DELTA_WIDTH: {
            _janelia.config._max_delta_width = value;
            _reinitializeJanelia();
            break;
        }
        case MAX_DELTA_OFFSET: {
            _janelia.config._max_delta_offset = value;
            _reinitializeJanelia();
            break;
        }
        case HALF_SPACE_ASSYMETRY_THRESH: {
            _janelia.config._half_space_assymetry = value;
            _reinitializeJanelia();
            break;
        }
        case HALF_SPACE_TUNNELING_MAX_MOVES: {
            _janelia.config._half_space_tunneling_max_moves = value;
            _reinitializeJanelia();
            break;
        }
    }
}

void WhiskerTracker::_removeWhiskersByWhiskerPadRadius(std::vector<Line2D> & whiskers)
{

    auto erase_inds = std::vector<std::size_t>();

    for (std::size_t i = 0; i < whiskers.size(); i++) {
        auto distance_to_follicle = distance(whiskers[i][0], _whisker_pad);

        if (distance_to_follicle > _whisker_pad_radius) {
            erase_inds.push_back(i);
        }
    }

    whisker::erase_whiskers(whiskers, erase_inds);
}

void WhiskerTracker::_reinitializeJanelia() {
    if (_janelia_init == false) {
        _janelia.bank = janelia::LineDetector(_janelia.config);
        _janelia.half_space_bank = janelia::HalfSpaceDetector(_janelia.config);
        _janelia_init = true;
    }
}

void WhiskerTracker::_connectToFaceMask(std::vector<Line2D> & whiskers)
{
    if (_face_mask.empty()) {
        return;
    }

    for (auto &w: whiskers) {

        whisker::extend_line_to_mask(w, _face_mask_set, _image_width, _image_height);
    }
}

/**
 * @brief Orders the whiskers based on their position.
 *
 * This function orders the whiskers with the most posterior being 0, more anterior 1, etc.
 * If the head direction vector is known, the follicular base can be projected onto this vector.
 * Consequently, the smallest value of the projection will be most posterior.
 *
 * The function first calculates the projection of each whisker's follicular base onto the head direction vector.
 * It then creates a vector of indices from 0 to the number of whiskers and sorts this vector based on the calculated projections.
 * Finally, it creates a new vector of whiskers sorted according to the calculated order and replaces the original vector of whiskers with this sorted vector.
 *
 * @param whiskers A vector of whiskers to be ordered.
 * @param head_direction_vector The head direction vector to project the whiskers onto.
 */
void order_whiskers(std::vector<Line2D> & whiskers, GeomVector const & head_direction_vector)
{
    std::vector<float> w_projection_vector;
    for (auto const & w : whiskers)
    {
        w_projection_vector.push_back(project(head_direction_vector, w[0]));
    }

    auto position_order = std::vector<std::size_t>(w_projection_vector.size());
    std::iota(position_order.begin(), position_order.end(), 0);
    std::sort(
            std::begin(position_order),
            std::end(position_order),
            [&](std::size_t i1, std::size_t i2)
            { return w_projection_vector[i1] > w_projection_vector[i2]; }
            );

    /*
    for (std::size_t i = 0; i < position_order.size(); i++) {

        std::cout << "The " << i << " position whisker is " << position_order[i];
        std::cout << " with follicle at " << "(" << whiskers[position_order[i]][0].x << ","
                << whiskers[position_order[i]][0].y << ")" << std::endl;
    }
    */

    std::vector<Line2D> sorted_whiskers;
    for (std::size_t i : position_order) {
        sorted_whiskers.push_back(whiskers[i]);
    }

    whiskers = sorted_whiskers;
}

}