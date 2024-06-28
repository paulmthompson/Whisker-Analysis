#include "whisker.hpp"

#include <cmath>
#include <iostream>
#include <algorithm>

namespace whisker
{



std::set<Point2D<int>> create_set(std::vector<Point2D<int>> const & points)
{
    return std::set<Point2D<int>>{points.begin(), points.end()};
}

std::set<Point2D<int>> create_set(std::vector<Point2D<float>> const & points)
{
    std::set<Point2D<int>> s;
    for (auto const & p : points) {
        s.insert(Point2D<int>{
            static_cast<int>(std::lround(p.x)),
            static_cast<int>(std::lround(p.y))});
    }
    return s;
}

bool intersect(Point2D<float> const p1, Mask2D const & mask)
{
    auto mask_set = create_set(mask);

    return intersect(p1, mask_set);
}

bool intersect(Point2D<float> const p1, std::set<Point2D<int>> const & mask_set)
{
    Point2D<int> p(
            static_cast<int>(std::lround(p1.x)),
            static_cast<int>(std::lround(p1.y))
            );

    return mask_set.find(p) != mask_set.end();
}

float calculate_overlap_iou(std::set<Point2D<int>> const & l1_set, std::set<Point2D<int>> const & l2_set)
{

    std::vector<whisker::Point2D<int>> v_intersection;
    std::vector<whisker::Point2D<int>> v_union;

    std::set_intersection(l1_set.begin(), l1_set.end(), l2_set.begin(), l2_set.end(),
                          std::back_inserter(v_intersection));

    std::set_union(l1_set.begin(), l1_set.end(), l2_set.begin(), l2_set.end(),
                   std::back_inserter(v_union));

    return static_cast<float>(v_intersection.size()) / static_cast<float>(v_union.size());
}

float calculate_overlap_iou(Line2D const& line, Line2D const& line2)
{
    auto l1_set = create_set(line);
    auto l2_set = create_set(line2);

    return calculate_overlap_iou(l1_set, l2_set);
}

float calculate_overlap_iou_relative(std::set<Point2D<int>> const & l1_set, std::set<Point2D<int>> const & l2_set)
{

    std::vector<whisker::Point2D<int>> v_intersection;

    std::set_intersection(l1_set.begin(), l1_set.end(), l2_set.begin(), l2_set.end(),
                          std::back_inserter(v_intersection));

    float iou_1 = static_cast<float>(v_intersection.size()) / static_cast<float>(l1_set.size());
    float iou_2 = static_cast<float>(v_intersection.size()) / static_cast<float>(l2_set.size());

    return std::max(iou_1, iou_2);
}

float calculate_overlap_iou_relative(Line2D const& line, Line2D const& line2)
{
    auto l1_set = create_set(line);
    auto l2_set = create_set(line2);

    return calculate_overlap_iou_relative(l1_set, l2_set);
}

void extend_line_to_mask(Line2D & line, std::set<Point2D<int>> const & mask, int const x_bound, int const y_bound)
{
    std::size_t vec_index = 5;
    const auto line_spacing = 2.0f;

    auto point = line[0];

    if (vec_index > line.size())
    {
        std::cout << "Requested index " << vec_index << " out of bounds" << std::endl;
        vec_index = 1;
    }

    auto reference_point = line[vec_index];
    auto dir_vector = create_vector(reference_point,point);

    dir_vector = normalize(dir_vector);

    point = create_point(dir_vector, point);
    reference_point = point;

    while(!whisker::intersect(point, mask)) {
        point = create_point(dir_vector, point);
        if (point.x < 0 || point.y < 0 || point.x > x_bound || point.y > y_bound)
        {
            break;
        }
    }

    auto extended_line = linspace(point, line[0], line_spacing);

    line.insert(line.begin(), extended_line.begin(), extended_line.end() - 1);

};

}