#ifndef WHISKER_ANALYSIS_LINES_HPP
#define WHISKER_ANALYSIS_LINES_HPP

#include "Geometry/points.hpp"

#include <vector>

namespace whisker {

struct Line2D : public std::vector<Point2D<float>> {
    using vector::vector;
};

inline Line2D create_line(std::vector<float> const & x, std::vector<float> const & y)
{
    auto line = Line2D();

    for (int i = 0; i < x.size(); i++) {
        line.push_back(Point2D<float>{x[i],y[i]});
    }
    return line;
}

inline float length(Line2D const & line)
{
    auto s = 0.0f;

    for (int i = 1; i < line.size(); i ++) {
        s += distance(line[i],line[i-1]);
    }

    return s;
}

inline float minimum_distance(Line2D const &line, Point2D<float> const p)
{
    auto dist = distance(line[0], p);

    for (int i = 1; i < line.size(); i ++) {
        auto this_dist = distance(line[i], p);
        if (this_dist < dist) {
            dist = this_dist;
        }
    }
    return dist;
}

}

#endif //WHISKER_ANALYSIS_LINES_HPP
