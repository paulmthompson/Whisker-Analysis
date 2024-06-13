#ifndef WHISKER_ANALYSIS_VECTOR_HPP
#define WHISKER_ANALYSIS_VECTOR_HPP

#include "points.hpp"

namespace whisker {

struct GeomVector {
    float x;
    float y;
};

template <typename T>
inline GeomVector create_vector(Point2D<T> p1, Point2D<T> p2)
{
    auto dx = p2.x - p1.x;
    auto dy = p2.y - p1.y;

    return GeomVector{static_cast<float>(dx),static_cast<float>(dy)};
};

inline GeomVector reverse(GeomVector vec) {
    return GeomVector{-1.0f * vec.x, -1.0f * vec.y};
};

inline float magnitude(GeomVector vec) {
    return static_cast<float>(std::sqrt(std::pow(vec.x ,2) + std::pow(vec.y,2)));
}

inline GeomVector normalize(GeomVector vec) {
    auto mag = magnitude(vec);
    return GeomVector{vec.x / mag, vec.y / mag};
}

}
#endif //WHISKER_ANALYSIS_VECTOR_HPP
