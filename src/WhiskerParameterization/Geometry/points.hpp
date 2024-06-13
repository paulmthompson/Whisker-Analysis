#ifndef WHISKER_ANALYSIS_POINTS_HPP
#define WHISKER_ANALYSIS_POINTS_HPP

#include <cmath>
#include <iostream>

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

template <typename T>
inline float distance(Point2D<T> const p1, Point2D<T> const p2)
{
    return static_cast<float>(std::sqrt(std::pow(p1.x - p2.x,2) + std::pow(p1.y - p2.y,2)));
};

template <typename T>
inline Point2D<float> point_along_path(Point2D<T> const p1, Point2D<T> const p2, float const pathlength)
{

    auto dist = distance(p1,p2);

    if (dist < pathlength) {
        std::cout << "Requested path length " << pathlength <<
                  " is larger than distance between points" << std::endl;
        return Point2D<float>{0.0f, 0.0f};
    } else if (pathlength <= 0.0f)
    {
        std::cout << "Invalid pathlength of " << pathlength << " specified" << std::endl;
        return Point2D<float>{0.0f, 0.0f};
    }

    auto ratio = pathlength / dist;

    auto p3_x = (p2.x - p1.x) * ratio + p1.x;
    auto p3_y = (p2.y - p1.y) * ratio + p1.y;

    return Point2D<float>{p3_x, p3_y};
};

}
#endif //WHISKER_ANALYSIS_POINTS_HPP
