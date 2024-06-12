#ifndef WHISKER_ANALYSIS_WHISKER_HPP
#define WHISKER_ANALYSIS_WHISKER_HPP

#include <vector>

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

//Point2D<float> point_at_pathlength(line2D const & line, float const pathlength);

}
#endif //WHISKER_ANALYSIS_WHISKER_HPP
