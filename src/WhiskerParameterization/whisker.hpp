#ifndef WHISKER_ANALYSIS_WHISKER_HPP
#define WHISKER_ANALYSIS_WHISKER_HPP

#include <vector>
#include <tuple>
#include <set>

#include "geometry.hpp"
#include "Geometry/points.hpp"
#include "Geometry/lines.hpp"
#include "Geometry/mask.hpp"
#include "Geometry/vector.hpp"

namespace whisker {

std::set<Point2D<int>> create_set(std::vector<Point2D<int>> const & points);

std::set<Point2D<int>> create_set(std::vector<Point2D<float>> const & points);

float calculate_overlap_iou(Line2D const& line, Line2D const& line2);
float calculate_overlap_iou(std::set<Point2D<int>> const & l1_set, std::set<Point2D<int>> const & l2_set);

float calculate_overlap_iou_relative(Line2D const& line, Line2D const& line2);
float calculate_overlap_iou_relative(std::set<Point2D<int>> const & l1_set, std::set<Point2D<int>> const & l2_set);

bool intersect(Point2D<float> const p1, Mask2D const & mask);
bool intersect(Point2D<float> const p1, std::set<Point2D<int>> const & mask_set);

}
#endif //WHISKER_ANALYSIS_WHISKER_HPP
