#ifndef WHISKER_ANALYSIS_WHISKER_HPP
#define WHISKER_ANALYSIS_WHISKER_HPP

#include <vector>
#include <tuple>
#include <set>

#include "geometry.hpp"
#include "Geometry/points.hpp"
#include "Geometry/lines.hpp"

namespace whisker {

std::tuple<int, float> nearest_preceding_index_along_path(Line2D const& line, float const pathlength);

Point2D<float> point_at_pathlength(Line2D const & line, float const pathlength);

std::set<Point2D<int>> create_set(std::vector<Point2D<int>> const & points);

std::set<Point2D<int>> create_set(std::vector<Point2D<float>> const & points);

float calculate_overlap_iou(Line2D const& line, Line2D const& line2);

float calculate_overlap_iou_relative(Line2D const& line, Line2D const& line2);
}
#endif //WHISKER_ANALYSIS_WHISKER_HPP
