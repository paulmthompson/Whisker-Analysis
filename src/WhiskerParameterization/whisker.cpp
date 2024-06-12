#include "whisker.hpp"

#include <cmath>

namespace whisker
{



float distance(Point2D<float> p1, Point2D<float> p2) {
    return std::sqrt(std::pow(p1.x - p2.x,2) + std::pow(p1.y - p2.y,2));
}

float length(Line2D const & line)
{
    auto s = 0.0f;

    for (int i = 1; i < line.size(); i ++) {
        s += distance(line[i],line[i-1]);
    }

    return s;
}

}