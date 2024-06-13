#include "whiskertracker.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include "JaneliaWhiskerTracker/io.hpp"

namespace whisker {

janelia::Image<uint8_t> bg = janelia::Image<uint8_t>(640, 480, std::vector<uint8_t>(640 * 480, 0));

WhiskerTracker::WhiskerTracker() :
        _whisker_length_threshold{75.0},
        _whisker_pad_radius{150.0},
        _janelia_init{false},
        _whisker_pad{0.0f, 0.0f} ,
        _image_height{480},
        _image_width{640}
{
    _janelia = janelia::JaneliaTracker();
    whiskers = std::vector<Line2D>{};
}

void WhiskerTracker::trace(const std::vector<uint8_t> &image, const int image_height, const int image_width) {

    if (this->_janelia_init == false) {
        this->_janelia.bank = janelia::LineDetector(this->_janelia.config);
        this->_janelia.half_space_bank = janelia::HalfSpaceDetector(this->_janelia.config);
        this->_janelia_init = true;
    }

    whiskers.clear();

    janelia::Image<uint8_t> img = janelia::Image<uint8_t>(image_width, image_height, image);
    std::vector<janelia::Whisker_Seg> j_segs = _janelia.find_segments(1, img, bg);

    int whisker_count = 1;
    for (auto &w_seg: j_segs) {
        auto whisker = create_line(std::move(w_seg.x), std::move(w_seg.y));
        if (length(whisker) > _whisker_length_threshold) {
            _alignWhiskerToFollicle(whisker);
            whiskers.push_back(whisker);
        }
    }

    _removeDuplicates();
    _connectToFaceMask();
    _removeWhiskersByWhiskerPadRadius();
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

std::map<int, std::vector<Line2D>> WhiskerTracker::load_janelia_whiskers(const std::string filename) {
    auto j_segs = janelia::load_binary_data(filename);

    auto output_whiskers = std::map<int, std::vector<Line2D>>();

    for (auto &w_seg: j_segs) {

        if (output_whiskers.find(w_seg.time) == output_whiskers.end()) { // Key doesn't exist
            output_whiskers[w_seg.time] = std::vector<Line2D>();
        }

        output_whiskers[w_seg.time].push_back(create_line(std::move(w_seg.x), std::move(w_seg.y)));

    }

    return output_whiskers;
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
void WhiskerTracker::_alignWhiskerToFollicle(Line2D &whisker) {

    auto start_distance = distance(whisker[0], _whisker_pad);

    auto end_distance = distance(whisker.back(), _whisker_pad);

    if (start_distance > end_distance) {
        std::reverse(whisker.begin(), whisker.end());
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
        double corr;
    };

    auto correlation_threshold = 0.2;

    auto cor_mat = std::vector<correlation_matrix>();

    for (int i = 0; i < whiskers.size(); i++) {

        for (int j = i + 1; j < whiskers.size(); j++) {

            auto this_cor = calculate_overlap_iou_relative(whiskers[i], whiskers[j]);

            cor_mat.push_back(correlation_matrix{i, j, this_cor});
        }
    }

    std::vector<int> erase_inds = std::vector<int>();
    for (int i = 0; i < cor_mat.size(); i++) {
        if (cor_mat[i].corr > correlation_threshold) {
            std::cout << "Whiskers " << cor_mat[i].i << " and " << cor_mat[i].j << " are the same" << std::endl;

            if (length(whiskers[cor_mat[i].i]) > length(whiskers[cor_mat[i].j])) {
                erase_inds.push_back(cor_mat[i].j);
            } else {
                erase_inds.push_back(cor_mat[i].i);
            }
        }
    }

    _eraseWhiskers(erase_inds);

}

void WhiskerTracker::_removeWhiskersByWhiskerPadRadius() {

    std::vector<int> erase_inds = std::vector<int>();

    for (int i = 0; i < whiskers.size(); i++) {
        auto distance_to_follicle = distance(whiskers[i][0], _whisker_pad);

        if (distance_to_follicle > _whisker_pad_radius) {
            erase_inds.push_back(i);
        }
    }

    _eraseWhiskers(erase_inds);
}

void WhiskerTracker::_eraseWhiskers(std::vector<int> &erase_inds) {
    std::sort(erase_inds.begin(), erase_inds.end(), std::greater<int>());
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
    if (_face_mask.size() == 0) {
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

}