
#include "Geometry/lines.hpp"
#include "Geometry/curvature.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <string>

/**
 * 
 * A parabola has defined curvature at every point according to the curve:
 * 
 * k(x) = 2 * a / (1 + (2*a*x + b)^2)^(3/2)
 * 
 * 
 */
TEST_CASE("FiniteDifferences - 1", "[Curvature]") {

    const float a = 1.0f;
    const float b = 0.0f;
    const float c = 0.0f;

    //Generate a parabola with a = 1, b = 0 over the range -10 to 10 with 100 points
    auto x = std::vector<float>(100);
    auto y = std::vector<float>(100);

    for (int i = 0; i < 100; i++) {
        x[i] = -10.0f + 0.2f * i;
        y[i] = a * x[i] * x[i] + b * x[i] + c;
    }

    auto line = whisker::create_line(x, y);

    auto index = 50;

    // (0.04 - 0.04) / (0.2 * 2) = 0 / 0.4 = 0.0

    auto fd = whisker::central_difference_first_derivative(line, index, 1);

    CHECK_THAT(fd, Catch::Matchers::WithinAbs(0.0f, 1e-6f));

    // Check that this still works when the window size is increased
    fd = whisker::central_difference_first_derivative(line, index, 2);

    CHECK_THAT(fd, Catch::Matchers::WithinAbs(0.0f, 1e-6f));

    // Create new line with index 51 deleted so that it is asymmetric
    x.erase(x.begin() + 51);
    y.erase(y.begin() + 51);

    line = whisker::create_line(x, y);

    // (x = -0.2, y = 0.04)
    // (x = 0.4, y = 0.16)
    // (0.16 - 0.04) / (0.6) = 0.12 / 0.6 = 0.2

    fd = whisker::central_difference_first_derivative(line, index, 1);

    CHECK_THAT(fd, Catch::Matchers::WithinRel(0.2f, 1e-3f));
}

TEST_CASE("Curvature - 1", "[Curvature]") {

    const float a = 1.0f;
    const float b = 0.0f;
    const float c = 0.0f;

    //Generate a parabola with a = 1, b = 0 over the range -10 to 10 with 100 points
    auto x = std::vector<float>(100);
    auto y = std::vector<float>(100);

    for (int i = 0; i < 100; i++) {
        x[i] = -10.0f + 0.2f * i;
        y[i] = a * x[i] * x[i] + b * x[i] + c;
    }

    auto line = whisker::create_line(x, y);

    auto index = 50;

    // (2 * a) / (1 + (2 * a * x + b)^2)^(3/2)
    // (2 * 1) / (1 + (2 * 1 * 0 + 0)^2)^(3/2) = 2 / 1 = 2

    auto curvature = whisker::calculate_curvature_finite_differences(line, index, 1);

    CHECK_THAT(curvature, Catch::Matchers::WithinRel(2.0f, 1e-3f));

    // Check that this still works when the window size is increased
    curvature = whisker::calculate_curvature_finite_differences(line, index, 2);

    CHECK_THAT(curvature, Catch::Matchers::WithinRel(2.0f, 1e-3f));

}