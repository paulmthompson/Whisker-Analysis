#ifndef WHISKER_ANALYSIS_LINES_HPP
#define WHISKER_ANALYSIS_LINES_HPP

#include "Geometry/points.hpp"
#include "Geometry/vector.hpp"

#include <tuple>
#include <vector>

namespace whisker {

struct Line2D : public std::vector<Point2D<float>> {
    using vector::vector;
};

/**
 * @brief Create a line from two vectors of x and y coordinates
 * @param x Vector of x coordinates
 * @param y Vector of y coordinates
 * @return Line2D
 */
Line2D create_line(std::vector<float> const & x, std::vector<float> const & y);

/**
 * @brief Generates a 2D line with a specified number of points between a start and end point.
 *
 * This function generates a 2D line by linearly interpolating between a start and end point.
 * The number of points in the line is specified by the user. The points are evenly spaced along the line.
 * If the number of points is less than 2, a line with only the start and end points is returned.
 *
 * @param start A Point2D<float> object representing the start point of the line.
 * @param end A Point2D<float> object representing the end point of the line.
 * @param num_points The number of points to be included in the line.
 * @return A Line2D object representing the generated line. It is a vector of Point2D<float> objects.
 */
Line2D linspace(Point2D<float> start, Point2D<float> end, int const num_points);

/**
 * @brief Generates a 2D line with points spaced at a specified distance between a start and end point.
 *
 * This function generates a 2D line by linearly interpolating between a start and end point.
 * The spacing between points in the line is specified by the user. The points are evenly spaced along the line.
 * If the total distance between the start and end points is less than the specified spacing, a line with only the start and end points is returned.
 *
 * @param start A Point2D<float> object representing the start point of the line.
 * @param end A Point2D<float> object representing the end point of the line.
 * @param spacing The distance between points to be included in the line.
 * @return A Line2D object representing the generated line. It is a vector of Point2D<float> objects.
 */
Line2D linspace(Point2D<float> start, Point2D<float> end, float const spacing);

///////////////////////////////////////////////////////////////////////////////


/**
 * @brief Calculates the total length of a 2D line.
 *
 * This function computes the total length of a 2D line by summing up the distances
 * between consecutive points in the line.
 *
 * @param line A Line2D object representing the 2D line. It is a vector of Point2D<float> objects.
 * @return The total length of the line as a float.
 */
float length(Line2D const & line);

/**
 * @brief Finds the index of the point on a 2D line that is closest to a specified path length along the line.
 *
 * This function calculates the cumulative distance along a 2D line from the first point to each subsequent point.
 * It stops when the cumulative distance exceeds the specified path length. The function then returns the index of the
 * last point before the cumulative distance exceeded the path length, and the cumulative distance up to that point.
 *
 * @param line A Line2D object representing the 2D line. It is a vector of Point2D<float> objects.
 * @param pathlength The path length along the line for which the nearest preceding point is to be found.
 * @return A tuple containing the index of the nearest preceding point and the cumulative distance up to that point.
 */
std::tuple<int, float> nearest_preceding_index_along_path(Line2D const & line, float const pathlength);

/**
 * @brief Finds the point on a 2D line that is closest to a specified path length along the line.
 *
 * This function calculates the cumulative distance along a 2D line from the first point to each subsequent point.
 * It stops when the cumulative distance exceeds the specified path length. The function then returns the point on the
 * line that is at the specified path length from the start of the line.
 * It linearly interpolates between the two points that are closest to the specified path length to find the point.
 *
 * @param line A Line2D object representing the 2D line. It is a vector of Point2D<float> objects.
 * @param pathlength The path length along the line for which the point is to be found.
 * @return A Point2D<float> object representing the point on the line that is at the specified path length from the start of the line.
 */
Point2D<float> point_at_pathlength(Line2D const & line, float const pathlength);


///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Calculates the minimum distance from a point to a 2D line.
 *
 * This function computes the minimum distance from a given point to any point on a 2D line.
 * The distance between the given point and each point on the line is calculated using the Euclidean distance formula.
 * The function iterates over all points on the line and keeps track of the smallest distance encountered.
 *
 * @param line A Line2D object representing the 2D line. It is a vector of Point2D<float> objects.
 * @param p A Point2D<float> object representing the point from which the minimum distance to the line is calculated.
 * @return The minimum distance from the point to the line as a float.
 */
float minimum_distance(Line2D const & line, Point2D<float> const p);

/**
 * @brief Finds the index of the point on a 2D line that is closest to a specified point.
 * 
 * This function calculates the distance between a specified point and each point on a 2D line.
 * It returns the index of the point on the line that is closest to the specified point.
 * 
 * @param line A Line2D object representing the 2D line. It is a vector of Point2D<float> objects.
 * @param p A Point2D<float> object representing the point for which the nearest point on the line is to be found.
 */
size_t minimum_distance_index(Line2D const & line, Point2D<float> const p);

/**
 * 
 * Modified from http://www.sunshine2k.de/coding/java/PointOnLine/PointOnLine.html
 * 
 */
Point2D<float> projected_point_on_line(Point2D<float> const p, Point2D<float> const a, Point2D<float> const b);

float minimum_distance_to_segment(Point2D<float> const p, Point2D<float> const a, Point2D<float> const b);

float minimum_distance_exact(Line2D const & line, Point2D<float> const p);

///////////////////////////////////////////////////////////////////////////////

void unit_linear_extend_base(Line2D & line, int vec_index = 5);

void unit_linear_extend_tip(Line2D & line, int vec_index = 5, bool normalize_vector = true);

}// namespace whisker

#endif//WHISKER_ANALYSIS_LINES_HPP
