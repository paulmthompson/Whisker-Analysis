#ifndef WHISKER_ANALYSIS_WHISKER_HPP
#define WHISKER_ANALYSIS_WHISKER_HPP

#include <vector>
#include <tuple>
#include <set>

#include "geometry.hpp"

namespace whisker {

using Line2D = std::vector<Point2D<float>>;

Line2D create_line(std::vector<float> const& x, std::vector<float> const & y);

float distance(Point2D<float> const p1, Point2D<float> const p2);

float length(Line2D const & line);

float minimum_distance(Line2D const &line, Point2D<float> const p);

Point2D<float> point_along_path(Point2D<float> const p1, Point2D<float> const p2, float const pathlength);

std::tuple<int, float> nearest_preceding_index_along_path(Line2D const& line, float const pathlength);

Point2D<float> point_at_pathlength(Line2D const & line, float const pathlength);

std::set<Point2D<int>> create_set(std::vector<Point2D<int>> const & points);

std::set<Point2D<int>> create_set(std::vector<Point2D<float>> const & points);

float calculate_overlap_iou(Line2D const& line, Line2D const& line2);

float calculate_overlap_iou_relative(Line2D const& line, Line2D const& line2);
}
#endif //WHISKER_ANALYSIS_WHISKER_HPP
