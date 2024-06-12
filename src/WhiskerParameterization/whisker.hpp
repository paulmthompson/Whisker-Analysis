#ifndef WHISKER_ANALYSIS_WHISKER_HPP
#define WHISKER_ANALYSIS_WHISKER_HPP

#include <vector>
#include <tuple>

namespace whisker {

template <typename T>
struct Point2D {
    T x;
    T y;
};

using Line2D = std::vector<Point2D<float>>;

float distance(Point2D<float> const p1, Point2D<float> const p2);

float length(Line2D const & line);

Point2D<float> point_along_path(Point2D<float> p1, Point2D<float> p2, float const pathlength);

std::tuple<int, float> nearest_preceding_index_along_path(Line2D const& line, float const pathlength);

Point2D<float> point_at_pathlength(Line2D const & line, float const pathlength);

}
#endif //WHISKER_ANALYSIS_WHISKER_HPP
