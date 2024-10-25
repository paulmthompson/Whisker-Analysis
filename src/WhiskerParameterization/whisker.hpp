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

/**
 * @brief Extends a 2D line to intersect with a mask.
 * 
 * This function extends a 2D line to intersect with a mask. It first calculates the direction vector of the line.
 * Then, it iteratively extends the line in the direction of the direction vector until it intersects with the mask.
 * 
 * @param line The 2D line to be extended.
 * @param mask The mask to intersect the line with.
 * @param x_bound The maximum x-coordinate of the mask.
 * @param y_bound The maximum y-coordinate of the mask.
 */
void extend_line_to_mask(Line2D & line, std::set<Point2D<int>> const & mask, int const x_bound, int const y_bound);

/**
 * @brief Removes duplicate whiskers from a vector of whiskers.
 * 
 * This function removes duplicate whiskers from a vector of whiskers.
 * It calculates the overlap between each pair of whiskers and removes the shorter whisker.
 * 
 * @param whiskers A vector of whiskers to be modified.
 */
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

std::tuple<float, int> get_nearest_whisker(std::vector<Line2D> & whiskers, float x_p, float y_p);

/**
 * @brief orient line so that the first index is closest to the follicle
 *
 * Measures the distance between the Point at one end of a whisker and Point
 * at the other end. The whisker is then flipped so that the first index is closest
 * to the follicle
 *
 *
 * @param whisker whisker to be checked
 */
void align_whisker_to_follicle(Line2D &whisker, whisker::Point2D<float> whisker_pad);

/**
 * @brief Orders the whiskers based on their position.
 *
 * This function orders the whiskers with the most posterior being 0, more anterior 1, etc.
 * If the head direction vector is known, the follicular base can be projected onto this vector.
 * Consequently, the smallest value of the projection will be most posterior.
 *
 * The function first calculates the projection of each whisker's follicular base onto the head direction vector.
 * It then creates a vector of indices from 0 to the number of whiskers and sorts this vector based on the calculated projections.
 * Finally, it creates a new vector of whiskers sorted according to the calculated order and replaces the original vector of whiskers with this sorted vector.
 *
 * @param whiskers A vector of whiskers to be ordered.
 * @param head_direction_vector The head direction vector to project the whiskers onto.
 */
void order_whiskers(std::vector<Line2D> & whiskers, GeomVector const & head_direction_vector);

/**
 * @brief Removes whiskers that are outside a specified radius from a point.
 * 
 * This function removes whiskers that are outside a specified radius from a point.
 * It iterates over the whiskers and calculates the distance between the whisker's base and the specified point.
 * If the distance is greater than the specified radius, the whisker is removed from the vector of whiskers.
 * 
 * @param whiskers A vector of whiskers to be modified.
 * @param whisker_pad The point from which the distance is calculated.
 * @param radius The radius outside of which whiskers are removed.
 */
void remove_whiskers_outside_radius(std::vector<Line2D> & whiskers, Point2D<float> whisker_pad, float radius);

} // namespace whisker

#endif //WHISKER_ANALYSIS_WHISKER_HPP
