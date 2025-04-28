#ifndef WHISKER_ANALYSIS_POINTS_HPP
#define WHISKER_ANALYSIS_POINTS_HPP

#include <utility>

namespace whisker {

template <typename T>
struct Point2D {
    T x;
    T y;

    Point2D() = default;

    Point2D(T _x, T _y) : x(_x), y(_y) {}

    // Copy constructor
    Point2D(const Point2D<T> &p) : x(p.x), y(p.y) {}

    // Move constructor (noexcept)
    Point2D(Point2D<T> &&p) noexcept : x(std::move(p.x)), y(std::move(p.y)) {}

    // Copy assignment operator
    Point2D& operator=(const Point2D& other) = default;

    // Move assignment operator (noexcept)
    Point2D& operator=(Point2D&& other) noexcept = default;

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
float distance(Point2D<T> const p1, Point2D<T> const p2);

template <typename T>
float distance2(Point2D<T> const p1, Point2D<T> const p2);

/**
 * @brief Calculates a point along the path between two points.
 *
 * This function calculates a point along the path between two points (p1 and p2) at a specified path length from p1.
 * The path length is specified by the user. If the path length is greater than the distance between the two points,
 * or if the path length is less than or equal to zero, an error message is printed and a point at the origin is returned.
 *
 * @param p1 A Point2D<T> object representing the first point.
 * @param p2 A Point2D<T> object representing the second point.
 * @param pathlength The path length from the first point at which the point is to be calculated.
 * @return A Point2D<float> object representing the point along the path at the specified path length from the first point.
 */
template <typename T>
Point2D<float> point_along_path(Point2D<T> const p1, Point2D<T> const p2, float const pathlength);


extern template struct Point2D<float>;

extern template float distance(Point2D<float> const p1, Point2D<float> const p2);
extern template float distance2(Point2D<float> const p1, Point2D<float> const p2);
extern template Point2D<float> point_along_path(Point2D<float> const p1, Point2D<float> const p2, float const pathlength);

}
#endif //WHISKER_ANALYSIS_POINTS_HPP
