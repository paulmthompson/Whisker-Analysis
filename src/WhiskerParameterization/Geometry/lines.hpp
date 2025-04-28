#ifndef WHISKER_ANALYSIS_LINES_HPP
#define WHISKER_ANALYSIS_LINES_HPP

#include "Geometry/points.hpp"
#include "Geometry/vector.hpp"

#include <iostream>
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
inline Line2D create_line(std::vector<float> const &x, std::vector<float> const &y) {
    auto line = Line2D();

    for (std::size_t i = 0; i < x.size(); i++) {
        line.push_back(Point2D<float>{x[i], y[i]});
    }
    return line;
}

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
inline Line2D linspace(Point2D<float> start, Point2D<float> end, int const num_points)
{
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
};

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
inline Line2D linspace(Point2D<float> start, Point2D<float> end, float const spacing)
{
    auto total_distance = distance(start, end);

    if (total_distance < spacing)
    {
        std::cout << "Spacing is greater than the total distance between the points" << std::endl;
        return Line2D{start, end};
    }

    auto num_points = static_cast<int>(total_distance / spacing);

    return linspace(start, end, num_points);
};

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
inline float length(Line2D const &line) {
    auto s = 0.0f;

    for (std::size_t i = 1; i < line.size(); i++) {
        s += distance(line[i], line[i - 1]);
    }

    return s;
}

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
inline std::tuple<int, float> nearest_preceding_index_along_path(Line2D const &line, float const pathlength) {
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
inline Point2D<float> point_at_pathlength(Line2D const &line, float const pathlength) {
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
inline float minimum_distance(Line2D const &line, Point2D<float> const p) {
    auto dist = distance(line[0], p);

    for (std::size_t i = 1; i < line.size(); i++) {
        auto this_dist = distance(line[i], p);
        if (this_dist < dist) {
            dist = this_dist;
        }
    }
    return dist;
}

/**
 * @brief Finds the index of the point on a 2D line that is closest to a specified point.
 * 
 * This function calculates the distance between a specified point and each point on a 2D line.
 * It returns the index of the point on the line that is closest to the specified point.
 * 
 * @param line A Line2D object representing the 2D line. It is a vector of Point2D<float> objects.
 * @param p A Point2D<float> object representing the point for which the nearest point on the line is to be found.
 */
inline size_t minimum_distance_index(Line2D const &line, Point2D<float> const p) {
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
inline Point2D<float> projected_point_on_line(Point2D<float> const p, Point2D<float> const a, Point2D<float> const b) {

    auto e1 = create_vector(a, b);
    auto e2 = create_vector(a, p);

    float val_dot_product = dot(e1, e2);

    float len2 = e1.x * e1.x + e1.y * e1.y;

    Point2D<float> projection = {a.x + (val_dot_product * e1.x) / len2, a.y + (val_dot_product * e1.y) / len2};

    return projection;
};

inline float minimum_distance_to_segment(Point2D<float> const p, Point2D<float> const a, Point2D<float> const b)
{
    auto projected_point = projected_point_on_line(p, a, b);

    return distance(p, projected_point);

};

inline float minimum_distance_exact(Line2D const & line, Point2D<float> const p) {

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

///////////////////////////////////////////////////////////////////////////////


inline void unit_linear_extend_base(Line2D &line, int vec_index = 5)
{
    if (vec_index > line.size())
    {
        std::cout << "Requested index " << vec_index << " out of bounds" << std::endl;
        vec_index = 1;
    }
    auto dir_vector = create_vector(line[vec_index],line[0]);

    dir_vector = normalize(dir_vector);

    auto it = line.begin();
    it = line.insert(it, create_point(dir_vector, line[0]));
};

inline void unit_linear_extend_tip(Line2D &line, int vec_index = 5, bool normalize_vector = true)
{
    if (line.size() - vec_index < 0)
    {
        std::cout << "Requested index " << vec_index << " out of bounds" << std::endl;
        vec_index = 1;
    }
    auto dir_vector = create_vector(line[line.size() - vec_index - 1], line.back());

    if (normalize_vector) {
        dir_vector = normalize(dir_vector);
    }

    line.push_back(create_point(dir_vector, line.back()));
};

}

#endif //WHISKER_ANALYSIS_LINES_HPP
