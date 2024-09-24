#ifndef WHISKER_ANALYSIS_WHISKER_HPP
#define WHISKER_ANALYSIS_WHISKER_HPP

#include "Geometry/points.hpp"
#include "Geometry/lines.hpp"

#include <vector>
#include <tuple>
#include <set>

namespace whisker {

struct Mask2D;

std::set<Point2D<int>> create_set(std::vector<Point2D<int>> const & points);

std::set<Point2D<int>> create_set(std::vector<Point2D<float>> const & points);

float calculate_overlap_iou(Line2D const& line, Line2D const& line2);
float calculate_overlap_iou(std::set<Point2D<int>> const & l1_set, std::set<Point2D<int>> const & l2_set);

float calculate_overlap_iou_relative(Line2D const& line, Line2D const& line2);
float calculate_overlap_iou_relative(std::set<Point2D<int>> const & l1_set, std::set<Point2D<int>> const & l2_set);

/**
 * @brief Checks if a given point intersects with a mask.
 *
 * This function takes a point and a mask as input. It first converts the mask into a set of points.
 * Then, it checks if the given point intersects with the set of points in the mask.
 *
 * @param p1 The point to check for intersection.
 * @param mask The mask to check for intersection with the point.
 * @return Returns true if the point intersects with the mask, false otherwise.
 */
bool intersect(Point2D<float> const p1, Mask2D const & mask);

/**
 * @brief Checks if a given point intersects with a set of points.
 *
 * This function takes a point and a set of points as input. It first rounds the coordinates of the point to the nearest integer.
 * Then, it checks if the rounded point is in the set of points.
 *
 * @param p1 The point to check for intersection.
 * @param mask_set The set of points to check for intersection with the point.
 * @return Returns true if the point is in the set of points, false otherwise.
 */
bool intersect(Point2D<float> const p1, std::set<Point2D<int>> const & mask_set);

void extend_line_to_mask(Line2D & line, std::set<Point2D<int>> const & mask, int const x_bound, int const y_bound);

void remove_duplicates(std::vector<Line2D> & whiskers);

/**
 * @brief Erases whiskers at specified indices.
 *
 * This function takes a vector of indices and erases the whiskers at these indices from the whiskers vector.
 * It first sorts the indices in descending order and removes any duplicates.
 * Then, it iterates over the sorted and unique indices and erases the whisker at each index.
 * Note that the indices are processed in descending order to prevent the erasure of a whisker from affecting the indices of subsequent whiskers to be erased.
 *
 * @param whiskers A vector of whiskers to be modified.
 * @param erase_inds A vector of indices of the whiskers to be erased.
 */
void erase_whiskers(std::vector<Line2D> & whiskers, std::vector<std::size_t> &erase_inds);

}
#endif //WHISKER_ANALYSIS_WHISKER_HPP
