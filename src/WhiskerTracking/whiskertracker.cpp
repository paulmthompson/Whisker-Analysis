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

    remove_whiskers_outside_radius(whiskers, _whisker_pad, _whisker_pad_radius);

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

} // namespace whisker