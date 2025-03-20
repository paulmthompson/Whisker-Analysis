#ifndef WHISKER_ANALYSIS_CURVATURE_HPP
#define WHISKER_ANALYSIS_CURVATURE

#include "Geometry/lines.hpp"
#include "Geometry/points.hpp"

#include "spdlog/spdlog.h"

#include <optional>

namespace whisker {

/**
 * @brief Calculates the first derivative of a 2D line using central differences.
 *
 * @param line The 2D line represented as a sequence of points.
 * @param index The index of the point at which to calculate the derivative.
 * @param window_size The window size for the central difference calculation (default: 3).  This determines how many points on either side are used.
 *
 * @return The calculated first derivative, or std::nullopt if the derivative cannot be calculated (e.g., index out of bounds, zero denominator).
 */
inline std::optional<float> central_difference_first_derivative(Line2D const & line, int const index, int const window_size = 3) {
    if (index < window_size || index >= static_cast<int>(line.size()) - window_size) {
        spdlog::error("Index out of bounds for central difference calculation.");
        return std::nullopt;
    }

    auto const p0 = line[index - window_size];
    auto const p1 = line[index];
    auto const p2 = line[index + window_size];

    auto dx1 = p1.x - p0.x;
    auto dx2 = p2.x - p1.x;

    float const epsilon = 1e-6f;

    if (std::abs(dx1 + dx2) < epsilon) {
        spdlog::warn("Denominator too small for central difference calculation.");
        return std::nullopt;
    }

    // TODO
    // if the distance between left and right side is extreme,
    // the derivative is not well defined
    // We should at least logg this

    auto dydx = (p2.y - p0.y) / (dx1 + dx2);

    return dydx;
};

/**
 * @brief Calculates the second derivative using central differences.
 *
 * @param line The 2D line.
 * @param index The index at which to calculate the derivative.
 * @param window_size The window size (default is 3).
 * 
 * @return std::optional<float> The calculated second derivative, or std::nullopt on error.
*/
float central_difference_second_derivative(Line2D const & line, size_t index, int const window_size = 3) {
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

/**
 * @brief Calculates curvature using finite differences.
 *
 * @param line The 2D line.
 * @param index The index at which to calculate the curvature.
 * @param window_size The window size.
 * @return The curvature, or 0.0f if the curvature is not defined.
*/
inline float calculate_curvature_finite_differences(Line2D const & line, int const index, int const window_size = 3) {

    if (index < window_size || index >= static_cast<int>(line.size()) - window_size) {
        spdlog::error("Index out of bounds for curvature calculation.");
        return 0.0f;
    }

    auto p0 = line[index - window_size];
    auto p1 = line[index];
    auto p2 = line[index + window_size];

    auto f_prime_opt = central_difference_first_derivative(line, index, window_size);
    if (!f_prime_opt) {
        spdlog::warn("First derivative could not be calculated, so curvature is undefined.");
        return 0.0f;
    }
    auto f_prime = f_prime_opt.value();

    float f_double_prime = central_difference_second_derivative(line, index, window_size);

    // Calculate curvature using the formula
    float curvature = std::abs(f_double_prime) / std::pow(1 + f_prime * f_prime, 1.5f);

    return curvature;
};


}// namespace whisker

#endif//WHISKER_ANALYSIS_CURVATURE_HPP