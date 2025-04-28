#ifndef WHISKER_ANALYSIS_CURVATURE_HPP
#define WHISKER_ANALYSIS_CURVATURE

#include "Geometry/lines.hpp"
#include "Geometry/points.hpp"

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
std::optional<float> central_difference_first_derivative(Line2D const & line, int const index, int const window_size = 3);
/**
 * @brief Calculates the second derivative using central differences.
 *
 * @param line The 2D line.
 * @param index The index at which to calculate the derivative.
 * @param window_size The window size (default is 3).
 * 
 * @return std::optional<float> The calculated second derivative, or std::nullopt on error.
*/
float central_difference_second_derivative(Line2D const & line, size_t index, int const window_size = 3);


/**
 * @brief Calculates curvature using finite differences.
 *
 * @param line The 2D line.
 * @param index The index at which to calculate the curvature.
 * @param window_size The window size.
 * @return The curvature, or 0.0f if the curvature is not defined.
*/
float calculate_curvature_finite_differences(Line2D const & line, int const index, int const window_size = 3);

}// namespace whisker

#endif//WHISKER_ANALYSIS_CURVATURE_HPP