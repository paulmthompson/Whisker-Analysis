#ifndef WHISKER_ANALYSIS_LINES_HPP
#define WHISKER_ANALYSIS_LINES_HPP

#include "Geometry/points.hpp"
#include "Geometry/vector.hpp"

#include <vector>
#include <tuple>

namespace whisker {

struct Line2D : public std::vector<Point2D<float>> {
    using vector::vector;
};

inline Line2D create_line(std::vector<float> const &x, std::vector<float> const &y) {
    auto line = Line2D();

    for (std::size_t i = 0; i < x.size(); i++) {
        line.push_back(Point2D<float>{x[i], y[i]});
    }
    return line;
}

inline float length(Line2D const &line) {
    auto s = 0.0f;

    for (std::size_t i = 1; i < line.size(); i++) {
        s += distance(line[i], line[i - 1]);
    }

    return s;
}

inline float minimum_distance(Line2D const &line, Point2D<float> const p) {
    auto dist = distance(line[0], p);

    for (std::size_t i = 1; i < line.size(); i++) {
        auto this_dist = distance(line[i], p);
        if (this_dist < dist) {
            dist = this_dist;
        }
    }
    return dist;
}

inline std::tuple<int, float> nearest_preceding_index_along_path(Line2D const &line, float const pathlength) {
    if (line.size() < 2) {
        std::cout << "Line does not contain at least 2 points" << std::endl;
        return std::make_tuple(0, 0.0f);
    } else if (length(line) < pathlength) {
        std::cout << "The requested pathlength is greater than the length of the line" << std::endl;
        return std::make_tuple(0, 0.0f);
    }

    float s = 0.0f;
    int closest_pre_ind = 0;

    for (std::size_t i = 1; i < line.size(); i++) {
        auto dist = distance(line[i], line[i - 1]);
        if (s + dist > pathlength) {
            break;
        } else {
            s = s + dist;
            closest_pre_ind += 1;
        }
    }

    return std::make_tuple(closest_pre_ind, s);
}

inline Point2D<float> point_at_pathlength(Line2D const &line, float const pathlength) {
    if (line.size() < 2) {
        std::cout << "Line does not contain at least 2 points" << std::endl;
        return Point2D<float>{0.0f, 0.0f};
    } else if (length(line) < pathlength) {
        std::cout << "The requested pathlength is greater than the length of the line" << std::endl;
        return line.back();
    }

    auto const [closest_pre_ind, dist] = nearest_preceding_index_along_path(line, pathlength);

    //auto final_segment_dist = distance(line[closest_pre_ind],line[closest_pre_ind+1]);

    return point_along_path(line[closest_pre_ind], line[closest_pre_ind + 1], pathlength - dist);
}

inline void unit_linear_extend_base(Line2D &line, int vec_index = 5)
{
    if (vec_index > line.size())
    {
        std::cout << "Requested index " << vec_index << " out of bounds" << std::endl;
        vec_index = 1;
    }
    auto dir_vector = create_vector(line[vec_index],line[0]);

    dir_vector = normalize(dir_vector);

    auto it = line.begin();
    it = line.insert(it, create_point(dir_vector, line[0]));
};

inline void unit_linear_extend_tip(Line2D &line, int vec_index = 5, bool normalize_vector = true)
{
    if (line.size() - vec_index < 0)
    {
        std::cout << "Requested index " << vec_index << " out of bounds" << std::endl;
        vec_index = 1;
    }
    auto dir_vector = create_vector(line[line.size() - vec_index - 1], line.back());

    if (normalize_vector) {
        dir_vector = normalize(dir_vector);
    }

    line.push_back(create_point(dir_vector, line.back()));
};

}

#endif //WHISKER_ANALYSIS_LINES_HPP
