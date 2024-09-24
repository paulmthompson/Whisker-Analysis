#include "whisker.hpp"

#include "Geometry/mask.hpp"
#include "Geometry/vector.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <numeric>

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
    Point2D<int> p{
            static_cast<int>(std::lround(p1.x)),
            static_cast<int>(std::lround(p1.y))
    };

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

}

void remove_duplicates(std::vector<Line2D> & whiskers) {

    struct correlation_matrix {
        int i;
        int j;
        float corr;
    };

    auto correlation_threshold = 0.2f;

    auto cor_mat = std::vector<correlation_matrix>();

    auto whisker_sets = std::vector<std::set<whisker::Point2D<int>>>();
    for (auto const & w : whiskers) {
        whisker_sets.push_back(whisker::create_set(w));
    }

    for (int i = 0; i < whisker_sets.size(); i++) {

        for (int j = i + 1; j < whisker_sets.size(); j++) {

            auto this_cor = calculate_overlap_iou_relative(whisker_sets[i], whisker_sets[j]);

            if (this_cor > correlation_threshold) {
                cor_mat.push_back(correlation_matrix{i, j, this_cor});
            }
        }
    }

    auto erase_inds = std::vector<std::size_t>();
    for (std::size_t i = 0; i < cor_mat.size(); i++) {
        //std::cout << "Whiskers " << cor_mat[i].i << " and " << cor_mat[i].j << " are the same" << std::endl;

        if (length(whiskers[cor_mat[i].i]) > length(whiskers[cor_mat[i].j])) {
            erase_inds.push_back(cor_mat[i].j);
        } else {
            erase_inds.push_back(cor_mat[i].i);
        }
    }

    erase_whiskers(whiskers, erase_inds);
}

void erase_whiskers(std::vector<Line2D> & whiskers, std::vector<std::size_t> & erase_inds)
{
    std::ranges::sort(erase_inds, std::greater<>());
    auto last = std::unique(erase_inds.begin(), erase_inds.end());
    erase_inds.erase(last, erase_inds.end());

    for (auto &erase_ind: erase_inds) {
        whiskers.erase(whiskers.begin() + erase_ind);
    }
}

std::tuple<float, int> get_nearest_whisker(std::vector<Line2D> & whiskers, float x_p, float y_p) {

    float nearest_distance = 1000.0;
    int whisker_id = 0;

    float current_d = 0.0f;
    int current_whisker_id = 0;

    for (auto &w: whiskers) {
        for (int i = 0; i < w.size(); i++) {
            current_d = sqrt(pow(x_p - w[i].x, 2) + pow(y_p - w[i].y, 2));
            if (current_d < nearest_distance) {
                nearest_distance = current_d;
                whisker_id = current_whisker_id;
            }
        }
        current_whisker_id += 1;
    }

    return std::make_tuple(nearest_distance, whisker_id);
}


void align_whisker_to_follicle(Line2D & whisker, whisker::Point2D<float> whisker_pad) {

    auto start_distance = distance(whisker[0], whisker_pad);

    auto end_distance = distance(whisker.back(), whisker_pad);

    if (start_distance > end_distance) {
        std::ranges::reverse(whisker);
    }
}

void order_whiskers(std::vector<Line2D> & whiskers, GeomVector const & head_direction_vector)
{
    std::vector<float> w_projection_vector;
    for (auto const & w : whiskers)
    {
        w_projection_vector.push_back(project(head_direction_vector, w[0]));
    }

    auto position_order = std::vector<std::size_t>(w_projection_vector.size());
    std::iota(position_order.begin(), position_order.end(), 0);
    std::sort(
            std::begin(position_order),
            std::end(position_order),
            [&](std::size_t i1, std::size_t i2)
            { return w_projection_vector[i1] > w_projection_vector[i2]; }
            );

    /*
    for (std::size_t i = 0; i < position_order.size(); i++) {

        std::cout << "The " << i << " position whisker is " << position_order[i];
        std::cout << " with follicle at " << "(" << whiskers[position_order[i]][0].x << ","
                << whiskers[position_order[i]][0].y << ")" << std::endl;
    }
    */

    std::vector<Line2D> sorted_whiskers;
    for (std::size_t i : position_order) {
        sorted_whiskers.push_back(whiskers[i]);
    }

    whiskers = sorted_whiskers;
}

void remove_whiskers_outside_radius(std::vector<Line2D> & whiskers, Point2D<float> whisker_pad, float radius)
{

    auto erase_inds = std::vector<std::size_t>();

    for (std::size_t i = 0; i < whiskers.size(); i++) {
        auto distance_to_follicle = distance(whiskers[i][0], whisker_pad);

        if (distance_to_follicle > radius) {
            erase_inds.push_back(i);
        }
    }

    whisker::erase_whiskers(whiskers, erase_inds);
}

} // namespace whisker