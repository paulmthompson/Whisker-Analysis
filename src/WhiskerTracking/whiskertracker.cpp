#include "whiskertracker.hpp"

#include "JaneliaWhiskerTracker/io.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>

namespace whisker {

janelia::Image<uint8_t> bg = janelia::Image<uint8_t>(640, 480, std::vector<uint8_t>(640 * 480, 0));

WhiskerTracker::WhiskerTracker()
{
    _janelia = janelia::JaneliaTracker();
    whiskers = std::vector<Line2D>{};
}

void WhiskerTracker::trace(const std::vector<uint8_t> &image, const int image_height, const int image_width) {

    if (_janelia_init == false) {
        _janelia.bank = janelia::LineDetector(_janelia.config);
        _janelia.half_space_bank = janelia::HalfSpaceDetector(_janelia.config);
        _janelia_init = true;
    }

    whiskers.clear();
    _position_order.clear();

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

    _removeDuplicates();
    std::ranges::for_each(whiskers, [wp=_whisker_pad](Line2D & w)
    {_alignWhiskerToFollicle(w, wp);});

    auto t4 = std::chrono::high_resolution_clock::now();

    _connectToFaceMask();

    auto t5 = std::chrono::high_resolution_clock::now();

    _removeWhiskersByWhiskerPadRadius();

    auto t6 = std::chrono::high_resolution_clock::now();

    _orderWhiskers();

    auto t7 = std::chrono::high_resolution_clock::now();

    std::cout << "Image conversion: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << "ms" << std::endl;
    std::cout << "Janelia find segments: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
    std::cout << "Create whiskers: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << "ms" << std::endl;
    std::cout << "Remove duplicates: " << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count() << "ms" << std::endl;
    std::cout << "Connect to face mask: " << std::chrono::duration_cast<std::chrono::milliseconds>(t5 - t4).count() << "ms" << std::endl;
    std::cout << "Remove whiskers by whisker pad radius: " << std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count() << "ms" << std::endl;
    std::cout << "Order whiskers: " << std::chrono::duration_cast<std::chrono::milliseconds>(t7 - t6).count() << "ms" << std::endl;

}

std::tuple<float, int> WhiskerTracker::get_nearest_whisker(float x_p, float y_p) {

    float nearest_distance = 1000.0;
    int whisker_id = 0;

    float current_d = 0.0f;
    int current_whisker_id = 0;

    for (auto &w: this->whiskers) {
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

std::map<int, std::vector<Line2D>> WhiskerTracker::load_janelia_whiskers(std::string const & filename) {
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
 * @brief WhiskerTracker::_alignWhiskerToFollicle
 *
 * Measures the distance between the Point at one end of a whisker and Point
 * at the other end. The whisker is then flipped so that the first index is closest
 * to the follicle
 *
 *
 * @param whisker whisker to be checked
 */
void _alignWhiskerToFollicle(Line2D &whisker, whisker::Point2D<float> whisker_pad) {

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

void WhiskerTracker::_removeDuplicates() {

    struct correlation_matrix {
        int i;
        int j;
        float corr;
    };

    auto correlation_threshold = 0.2f;

    auto cor_mat = std::vector<correlation_matrix>();

    auto whisker_sets = std::vector<std::set<whisker::Point2D<int>>>();
    for (auto const & w : whiskers) {
        whisker_sets.push_back(whisker::create_set(w));
    }

    for (int i = 0; i < whisker_sets.size(); i++) {

        for (int j = i + 1; j < whisker_sets.size(); j++) {

            auto this_cor = calculate_overlap_iou_relative(whisker_sets[i], whisker_sets[j]);

            if (this_cor > correlation_threshold) {
                cor_mat.push_back(correlation_matrix{i, j, this_cor});
            }
        }
    }

    auto erase_inds = std::vector<int>();
    for (int i = 0; i < cor_mat.size(); i++) {
        //std::cout << "Whiskers " << cor_mat[i].i << " and " << cor_mat[i].j << " are the same" << std::endl;

        if (length(whiskers[cor_mat[i].i]) > length(whiskers[cor_mat[i].j])) {
            erase_inds.push_back(cor_mat[i].j);
        } else {
            erase_inds.push_back(cor_mat[i].i);
        }
    }

    _eraseWhiskers(erase_inds);
}

void WhiskerTracker::_removeWhiskersByWhiskerPadRadius() {

    auto erase_inds = std::vector<int>();

    for (int i = 0; i < whiskers.size(); i++) {
        auto distance_to_follicle = distance(whiskers[i][0], _whisker_pad);

        if (distance_to_follicle > _whisker_pad_radius) {
            erase_inds.push_back(i);
        }
    }

    _eraseWhiskers(erase_inds);
}

void WhiskerTracker::_eraseWhiskers(std::vector<int> &erase_inds) {
    std::ranges::sort(erase_inds, std::greater<int>());
    auto last = std::unique(erase_inds.begin(), erase_inds.end());
    erase_inds.erase(last, erase_inds.end());

    for (auto &erase_ind: erase_inds) {
        whiskers.erase(whiskers.begin() + erase_ind);
    }
}

void WhiskerTracker::_reinitializeJanelia() {
    _janelia.bank = janelia::LineDetector(_janelia.config);
    _janelia.half_space_bank = janelia::HalfSpaceDetector(_janelia.config);
    _janelia_init = true;
}

void WhiskerTracker::_connectToFaceMask()
{
    if (_face_mask.empty()) {
        return;
    }

    for (auto &w: this->whiskers) {
        while (!whisker::intersect(w[0], _face_mask)) {

            whisker::unit_linear_extend_base(w);

            if (w[0].x < 0 || w[0].y < 0 || w[0].x > _image_width || w[0].y > _image_height)
            {
                break;
            }
        }
    }
}

/**
 *
 * Whiskers are ordered with the most posterior being 0, more anterior 1, etc
 * If the head director vector is known, the follicular base can be projected
 * onto this vector. Consequently, the smallest value of the projection will be
 * most posterior.
 *
 *
 *
 */
void WhiskerTracker::_orderWhiskers()
{
    std::vector<float> w_projection_vector;
    for (auto const & w : whiskers)
    {
        w_projection_vector.push_back(project(_head_direction_vector, w[0]));
    }

    _position_order = std::vector<std::size_t>(w_projection_vector.size());
    std::iota(_position_order.begin(), _position_order.end(), 0);
    std::sort(
            std::begin(_position_order),
            std::end(_position_order),
            [&](std::size_t i1, std::size_t i2)
            { return w_projection_vector[i1] > w_projection_vector[i2]; }
            );

    for (std::size_t i = 0; i < _position_order.size(); i++) {

        std::cout << "The " << i << " position whisker is " << _position_order[i];
        std::cout << " with follicle at " << "(" << whiskers[_position_order[i]][0].x << ","
                  << whiskers[_position_order[i]][0].y << ")" << std::endl;
    }
}

}