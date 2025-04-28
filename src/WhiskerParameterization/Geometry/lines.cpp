
#include "lines.hpp"

#include <iostream>

namespace whisker {

Line2D create_line(std::vector<float> const & x, std::vector<float> const & y) {
    auto line = Line2D();

    for (std::size_t i = 0; i < x.size(); i++) {
        line.push_back(Point2D<float>{x[i], y[i]});
    }
    return line;
}

Line2D linspace(Point2D<float> start, Point2D<float> end, int const num_points) {
    if (num_points < 2) {
        std::cout << "Number of points must be greater than 1" << std::endl;
        return Line2D{start, end};
    }

    Line2D line;
    float dx = (end.x - start.x) / (num_points - 1);
    float dy = (end.y - start.y) / (num_points - 1);
    for (int i = 0; i < num_points; ++i) {
        line.push_back({start.x + i * dx, start.y + i * dy});
    }
    return line;
}

Line2D linspace(Point2D<float> start, Point2D<float> end, float const spacing) {
    auto total_distance = distance(start, end);

    if (total_distance < spacing) {
        std::cout << "Spacing is greater than the total distance between the points" << std::endl;
        return Line2D{start, end};
    }

    auto num_points = static_cast<int>(total_distance / spacing);

    return linspace(start, end, num_points);
}

float length(Line2D const & line) {
    auto s = 0.0f;

    for (std::size_t i = 1; i < line.size(); i++) {
        s += distance(line[i], line[i - 1]);
    }

    return s;
}


std::tuple<int, float> nearest_preceding_index_along_path(Line2D const & line, float const pathlength) {
    if (line.size() < 2) {
        std::cout << "Line does not contain at least 2 points" << std::endl;
        return std::make_tuple(0, 0.0f);
    } else if (length(line) < pathlength) {
        std::cout << "The requested pathlength is greater than the length of the line" << std::endl;
        return std::make_tuple(0, 0.0f);
    }

    float s = 0.0f;
    int closest_pre_ind = 0;

    for (std::size_t i = 1; i < line.size(); i++) {
        auto dist = distance(line[i], line[i - 1]);
        if (s + dist > pathlength) {
            break;
        } else {
            s = s + dist;
            closest_pre_ind += 1;
        }
    }

    return std::make_tuple(closest_pre_ind, s);
}

Point2D<float> point_at_pathlength(Line2D const & line, float const pathlength) {
    if (line.size() < 2) {
        std::cout << "Line does not contain at least 2 points" << std::endl;
        return Point2D<float>{0.0f, 0.0f};
    } else if (length(line) < pathlength) {
        std::cout << "The requested pathlength is greater than the length of the line" << std::endl;
        return line.back();
    }

    auto const [closest_pre_ind, dist] = nearest_preceding_index_along_path(line, pathlength);

    //auto final_segment_dist = distance(line[closest_pre_ind],line[closest_pre_ind+1]);

    return point_along_path(line[closest_pre_ind], line[closest_pre_ind + 1], pathlength - dist);
}

float minimum_distance(Line2D const & line, Point2D<float> const p) {
    auto dist = distance(line[0], p);

    for (std::size_t i = 1; i < line.size(); i++) {
        auto this_dist = distance(line[i], p);
        if (this_dist < dist) {
            dist = this_dist;
        }
    }
    return dist;
}

size_t minimum_distance_index(Line2D const & line, Point2D<float> const p) {
    auto dist = distance(line[0], p);
    size_t index = 0;

    for (std::size_t i = 1; i < line.size(); i++) {
        auto this_dist = distance(line[i], p);
        if (this_dist < dist) {
            dist = this_dist;
            index = i;
        }
    }
    return index;
}

/**
 *
 * Modified from http://www.sunshine2k.de/coding/java/PointOnLine/PointOnLine.html
 *
 */
Point2D<float> projected_point_on_line(Point2D<float> const p, Point2D<float> const a, Point2D<float> const b) {

    auto e1 = create_vector(a, b);
    auto e2 = create_vector(a, p);

    float val_dot_product = dot(e1, e2);

    float len2 = e1.x * e1.x + e1.y * e1.y;

    Point2D<float> projection = {a.x + (val_dot_product * e1.x) / len2, a.y + (val_dot_product * e1.y) / len2};

    return projection;
}

float minimum_distance_to_segment(Point2D<float> const p, Point2D<float> const a, Point2D<float> const b) {
    auto projected_point = projected_point_on_line(p, a, b);

    return distance(p, projected_point);
}

float minimum_distance_exact(Line2D const & line, Point2D<float> const p) {

    // Find index of minimum distance from existing points
    auto min_index = minimum_distance_index(line, p);

    // Find minimum distance from line segment between min_index and min_index + 1
    // as well as min_index -1 and min_index

    if (min_index == 0) {
        return minimum_distance_to_segment(p, line[0], line[1]);
    } else if (min_index == line.size() - 1) {
        return minimum_distance_to_segment(p, line[min_index - 1], line[min_index]);
    }

    auto min_dist = minimum_distance_to_segment(p, line[min_index - 1], line[min_index]);
    auto dist = minimum_distance_to_segment(p, line[min_index], line[min_index + 1]);

    if (dist < min_dist) {
        min_dist = dist;
    }

    return min_dist;
}

void unit_linear_extend_base(Line2D & line, int vec_index) {
    if (vec_index > line.size()) {
        std::cout << "Requested index " << vec_index << " out of bounds" << std::endl;
        vec_index = 1;
    }
    auto dir_vector = create_vector(line[vec_index], line[0]);

    dir_vector = normalize(dir_vector);

    auto it = line.begin();
    it = line.insert(it, create_point(dir_vector, line[0]));
}

void unit_linear_extend_tip(Line2D & line, int vec_index, bool normalize_vector) {
    if (line.size() - vec_index < 0) {
        std::cout << "Requested index " << vec_index << " out of bounds" << std::endl;
        vec_index = 1;
    }
    auto dir_vector = create_vector(line[line.size() - vec_index - 1], line.back());

    if (normalize_vector) {
        dir_vector = normalize(dir_vector);
    }

    line.push_back(create_point(dir_vector, line.back()));
}

}// namespace whisker
