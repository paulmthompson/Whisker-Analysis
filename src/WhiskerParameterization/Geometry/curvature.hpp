#ifndef WHISKER_ANALYSIS_CURVATURE_HPP
#define WHISKER_ANALYSIS_CURVATURE

#include "Geometry/points.hpp"
#include "Geometry/lines.hpp"


namespace whisker {

inline float central_difference_first_derivative(Line2D const & line, int const index, int const window_size = 3)
{
    if (index < window_size || index >= line.size() - window_size) {
        std::cout << "Index out of bounds" << std::endl;
        return 0.0f;
    }

    auto p0 = line[index - window_size];
    auto p1 = line[index];
    auto p2 = line[index + window_size];

    auto dx1 = p1.x - p0.x;
    auto dx2 = p2.x - p1.x;

    auto dydx = (p2.y - p0.y) / (dx1 + dx2);

    return dydx;
};


float central_difference_second_derivative(Line2D const &line, size_t index, int const window_size = 3) 
{
     if (index < window_size || index >= line.size() - window_size) {
        std::cerr << "Central difference cannot be calculated at the endpoints." << std::endl;
        return 0.0f;
    }

    auto p0 = line[index - window_size];
    auto p1 = line[index];
    auto p2 = line[index + window_size];

    auto dx1 = p1.x - p0.x;
    auto dx2 = p2.x - p1.x;

    auto dy2dx2 = (p2.y - 2 * p1.y + p0.y) / (dx1 * dx2);

    return dy2dx2;
};

inline float calculate_curvature_finite_differences(Line2D const &line, int const index, int const window_size = 3) {

    if (index < window_size || index >= line.size() - window_size) {
        std::cout << "Index out of bounds" << std::endl;
        return 0.0f;
    }

    auto p0 = line[index - window_size];
    auto p1 = line[index];
    auto p2 = line[index + window_size];

    float f_prime = central_difference_first_derivative(line, index, window_size);

    float f_double_prime = central_difference_second_derivative(line, index, window_size);

    // Calculate curvature using the formula
    float curvature = std::abs(f_double_prime) / std::pow(1 + f_prime * f_prime, 1.5f);

    return curvature;   
};


} // namespace whisker

#endif //WHISKER_ANALYSIS_CURVATURE_HPP