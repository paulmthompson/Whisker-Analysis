
#include "vector.hpp"

#include <cmath>

namespace whisker {


template<typename T>
GeomVector create_vector(Point2D<T> p1, Point2D<T> p2) {
    auto dx = p2.x - p1.x;
    auto dy = p2.y - p1.y;

    return GeomVector{static_cast<float>(dx), static_cast<float>(dy)};
}

float dot(GeomVector vec1, GeomVector vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

GeomVector reverse(GeomVector vec) {
    return GeomVector{-1.0f * vec.x, -1.0f * vec.y};
}

float magnitude(GeomVector vec) {
    return static_cast<float>(std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2)));
}

template<typename T>
float project(GeomVector vec, Point2D<T> p2) {
    float a_dot_b = vec.x * static_cast<float>(p2.x) + vec.y * static_cast<float>(p2.y);

    return a_dot_b / magnitude(vec);
}

GeomVector normalize(GeomVector vec) {
    auto mag = magnitude(vec);
    return GeomVector{vec.x / mag, vec.y / mag};
}

Point2D<float> create_point(GeomVector vec, Point2D<float> p1) {
    return Point2D<float>{p1.x + vec.x, p1.y + vec.y};
}

template GeomVector create_vector(Point2D<float> p1, Point2D<float> p2);
template float project(GeomVector vec, Point2D<float> p2);

}// namespace whisker