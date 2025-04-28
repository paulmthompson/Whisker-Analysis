#ifndef WHISKER_ANALYSIS_VECTOR_HPP
#define WHISKER_ANALYSIS_VECTOR_HPP

#include "points.hpp"

namespace whisker {

struct GeomVector {
    float x;
    float y;

    // Default constructor
    GeomVector() noexcept = default;

    // Parameterized constructor
    GeomVector(float _x, float _y) noexcept
        : x(_x),
          y(_y) {}

    // Copy constructor
    GeomVector(GeomVector const & other) noexcept = default;

    // Move constructor
    GeomVector(GeomVector && other) noexcept = default;

    // Copy assignment operator
    GeomVector & operator=(GeomVector const & other) noexcept = default;

    // Move assignment operator
    GeomVector & operator=(GeomVector && other) noexcept = default;
};

/**
 * @brief Creates a geometric vector from two points.
 *
 * This function creates a geometric vector from two points (p1 and p2). The vector is directed from p1 to p2.
 *
 * @param p1 A Point2D<T> object representing the first point.
 * @param p2 A Point2D<T> object representing the second point.
 * @return A GeomVector object representing the vector from p1 to p2.
 */
template<typename T>
GeomVector create_vector(Point2D<T> p1, Point2D<T> p2);

float dot(GeomVector vec1, GeomVector vec2);

/**
 * @brief Reverses the direction of a geometric vector.
 *
 * This function reverses the direction of a geometric vector.
 * The magnitude of the vector remains the same, but its direction is reversed.
 *
 * @param vec A GeomVector object representing the vector to be reversed.
 * @return A GeomVector object representing the reversed vector.
 */
GeomVector reverse(GeomVector vec);

/**
 * @brief Calculates the magnitude of a geometric vector.
 *
 * This function calculates the magnitude (or length) of a geometric vector.
 *
 * @param vec A GeomVector object representing the vector whose magnitude is to be calculated.
 * @return A float representing the magnitude of the vector.
 */
float magnitude(GeomVector vec);

/**
 * @brief Projects a point onto a geometric vector.
 *
 * This function projects a point onto a geometric vector.
 * The projection is calculated as the dot product of the vector and the point, divided by the magnitude of the vector.
 *
 * @param vec A GeomVector object representing the vector onto which the point is to be projected.
 * @param p2 A Point2D<T> object representing the point to be projected.
 * @return A float representing the scalar projection of the point onto the vector.
 */
template<typename T>
float project(GeomVector vec, Point2D<T> p2);

/**
 * @brief Normalizes a geometric vector.
 *
 * This function normalizes a geometric vector. The normalized vector has the same direction
 * as the original vector, but its magnitude (or length) is 1.
 *
 * @param vec A GeomVector object representing the vector to be normalized.
 * @return A GeomVector object representing the normalized vector.
 */
GeomVector normalize(GeomVector vec);

/**
 * @brief Creates a point by adding a geometric vector to an existing point.
 *
 * This function creates a new point by adding the x and y components of a geometric vector
 * to the x and y coordinates of an existing point, respectively.
 * The new point is located at the end of the vector if the vector is placed such that its start coincides with the existing point.
 *
 * @param vec A GeomVector object representing the vector to be added to the point.
 * @param p1 A Point2D<float> object representing the existing point.
 * @return A Point2D<float> object representing the new point.
 */
Point2D<float> create_point(GeomVector vec, Point2D<float> p1);


extern template GeomVector create_vector(Point2D<float> p1, Point2D<float> p2);
extern template float project(GeomVector vec, Point2D<float> p2);

}// namespace whisker

#endif//WHISKER_ANALYSIS_VECTOR_HPP
