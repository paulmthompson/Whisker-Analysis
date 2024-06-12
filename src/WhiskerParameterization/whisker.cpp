#include "whisker.hpp"

#include <cmath>
#include <iostream>

namespace whisker
{



float distance(Point2D<float> const p1, Point2D<float> const p2) {
    return std::sqrt(std::pow(p1.x - p2.x,2) + std::pow(p1.y - p2.y,2));
}

float length(Line2D const & line)
{
    auto s = 0.0f;

    for (int i = 1; i < line.size(); i ++) {
        s += distance(line[i],line[i-1]);
    }

    return s;
}

Point2D<float> point_along_path(Point2D<float> const p1, Point2D<float> const p2, float const pathlength)
{

    auto dist = distance(p1,p2);

    if (dist < pathlength) {
        std::cout << "Requested path length " << pathlength <<
                  " is larger than distance between points" << std::endl;
        return Point2D<float>{0.0f, 0.0f};
    } else if (pathlength <= 0.0f)
    {
        std::cout << "Invalid pathlength of " << pathlength << " specified" << std::endl;
        return Point2D<float>{0.0f, 0.0f};
    }

    auto ratio = pathlength / dist;

    auto p3_x = (p2.x - p1.x) * ratio + p1.x;
    auto p3_y = (p2.y - p1.y) * ratio + p1.y;

    return Point2D{p3_x, p3_y};
}

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

}