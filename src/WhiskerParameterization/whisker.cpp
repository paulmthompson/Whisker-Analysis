#include "whisker.hpp"

#include <cmath>

namespace whisker
{



float distance(Point2D<float> p1, Point2D<float> p2) {
    return std::sqrt(std::pow(p1.x - p2.x,2) + std::pow(p1.y - p2.y,2));
}

}