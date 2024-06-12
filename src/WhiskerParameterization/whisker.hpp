#ifndef WHISKER_ANALYSIS_WHISKER_HPP
#define WHISKER_ANALYSIS_WHISKER_HPP

#include <vector>
#include <tuple>
#include <set>

namespace whisker {

template <typename T>
struct Point2D {
    T x;
    T y;

    friend bool operator< (Point2D<T> const & p1, Point2D<T> const & p2)
    {
        if (p1.x != p2.x) {
            return p1.x < p2.x;
        } else {
            return p1.y < p2.y;
        }
    }
};

using Line2D = std::vector<Point2D<float>>;

float distance(Point2D<float> const p1, Point2D<float> const p2);

float length(Line2D const & line);

Point2D<float> point_along_path(Point2D<float> const p1, Point2D<float> const p2, float const pathlength);

std::tuple<int, float> nearest_preceding_index_along_path(Line2D const& line, float const pathlength);

Point2D<float> point_at_pathlength(Line2D const & line, float const pathlength);

std::set<Point2D<int>> create_set(std::vector<Point2D<int>> const & points);

std::set<Point2D<int>> create_set(std::vector<Point2D<float>> const & points);
}
#endif //WHISKER_ANALYSIS_WHISKER_HPP
