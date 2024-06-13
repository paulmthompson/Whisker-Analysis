#ifndef WHISKER_ANALYSIS_VECTOR_HPP
#define WHISKER_ANALYSIS_VECTOR_HPP

#include "points.hpp"

namespace whisker {

struct GeomVector {
    float x;
    float y;
};

/**
 *
 * Creates vector point from p1 to p2
 *
 * @tparam T
 * @param p1
 * @param p2
 * @return
 */
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

inline Point2D<float> create_point(GeomVector vec, Point2D<float> p1)
{
  return Point2D<float>{p1.x + vec.x, p1.y + vec.y};
};

}
#endif //WHISKER_ANALYSIS_VECTOR_HPP
