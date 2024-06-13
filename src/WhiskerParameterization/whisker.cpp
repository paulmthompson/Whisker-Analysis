#include "whisker.hpp"

#include <cmath>
#include <iostream>
#include <algorithm>

namespace whisker
{



std::tuple<int, float> nearest_preceding_index_along_path(Line2D const& line, float const pathlength)
{
    if (line.size() < 2)
    {
        std::cout << "Line does not contain at least 2 points" << std::endl;
        return std::make_tuple(0,0.0f);
    } else if (length(line) < pathlength) {
        std::cout << "The requested pathlength is greater than the length of the line" << std::endl;
        return std::make_tuple(0,0.0f);
    }

    float s = 0.0f;
    int closest_pre_ind = 0;

    for (int i = 1; i < line.size(); i ++) {
        auto dist = distance(line[i],line[i-1]);
        if (s + dist > pathlength) {
            break;
        } else {
            s = s + dist;
            closest_pre_ind += 1;
        }
    }

    return std::make_tuple(closest_pre_ind, s);
}


Point2D<float> point_at_pathlength(Line2D const & line, float const pathlength)
{
    if (line.size() < 2) {
        std::cout << "Line does not contain at least 2 points" << std::endl;
        return Point2D{0.0f, 0.0f};
    } else if (length(line) < pathlength) {
        std::cout << "The requested pathlength is greater than the length of the line" << std::endl;
        return line.back();
    }

    auto const [closest_pre_ind, dist] = nearest_preceding_index_along_path(line, pathlength);

    //auto final_segment_dist = distance(line[closest_pre_ind],line[closest_pre_ind+1]);

    return point_along_path(line[closest_pre_ind],line[closest_pre_ind+1], pathlength - dist);
}

std::set<Point2D<int>> create_set(std::vector<Point2D<int>> const & points)
{
    return std::set<Point2D<int>>{points.begin(), points.end()};
}

std::set<Point2D<int>> create_set(std::vector<Point2D<float>> const & points)
{
    std::set<Point2D<int>> s;
    for (auto const & p : points) {
        s.insert(Point2D<int>{std::lround(p.x),std::lround(p.y)});
    }
    return s;
}

float calculate_overlap_iou(Line2D const& line, Line2D const& line2)
{
    auto l1_set = create_set(line);
    auto l2_set = create_set(line2);

    std::vector<whisker::Point2D<int>> v_intersection;
    std::vector<whisker::Point2D<int>> v_union;

    std::set_intersection(l1_set.begin(), l1_set.end(), l2_set.begin(), l2_set.end(),
                          std::back_inserter(v_intersection));

    std::set_union(l1_set.begin(), l1_set.end(), l2_set.begin(), l2_set.end(),
                          std::back_inserter(v_union));

    return static_cast<float>(v_intersection.size()) / static_cast<float>(v_union.size());
}

float calculate_overlap_iou_relative(Line2D const& line, Line2D const& line2)
{
    auto l1_set = create_set(line);
    auto l2_set = create_set(line2);

    std::vector<whisker::Point2D<int>> v_intersection;

    std::set_intersection(l1_set.begin(), l1_set.end(), l2_set.begin(), l2_set.end(),
                          std::back_inserter(v_intersection));

    float iou_1 = static_cast<float>(v_intersection.size()) / static_cast<float>(l1_set.size());
    float iou_2 = static_cast<float>(v_intersection.size()) / static_cast<float>(l2_set.size());

    return std::max(iou_1, iou_2);
}

}