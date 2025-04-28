
#include "Geometry/points.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("Point distance calculations", "[points][distance]") {

    auto p1 = whisker::Point2D<float>{0.0f, 0.0f};
    auto p2 = whisker::Point2D<float>{0.0f, 10.0f};
    auto p3 = whisker::Point2D<float>{10.0f, 10.0f};

    SECTION("Distance between two points") {
        REQUIRE(whisker::distance(p1, p2) == 10.0f);
    }

    SECTION("Distance between different points") {
        REQUIRE_THAT(whisker::distance(p1, p3), Catch::Matchers::WithinRel(14.142f, 1e-3f));
    }

    SECTION("Distance from point to itself") {
        REQUIRE(whisker::distance(p1, p1) == 0.0f);
    }
}

TEST_CASE("Finding intermediate points along a path", "[points][interpolation]") {

    auto p1 = whisker::Point2D<float>{0.0f, 0.0f};
    auto p2 = whisker::Point2D<float>{0.0f, 10.0f};
    auto p4 = whisker::Point2D<float>{6.0f, 8.0f};

    SECTION("Point halfway along vertical line") {
        auto intermediate_point = point_along_path(p1, p2, 5.0f);
        REQUIRE(intermediate_point.x == 0.0f);
        REQUIRE(intermediate_point.y == 5.0f);
    }

    SECTION("Point along diagonal line") {
        auto intermediate_point = point_along_path(p1, p4, 5.0f);
        REQUIRE(intermediate_point.x == 3.0f);
        REQUIRE(intermediate_point.y == 4.0f);
    }
}

TEST_CASE("Edge cases for point operations", "[points][edge_cases]") {

    auto p1 = whisker::Point2D<float>{0.0f, 0.0f};
    auto p2 = whisker::Point2D<float>{0.0f, 10.0f};

    SECTION("Distance beyond the end point") {
        auto intermediate_point = point_along_path(p1, p2, 15.0f);
        REQUIRE(intermediate_point.x == 0.0f);
        REQUIRE(intermediate_point.y == 0.0f);
    }

    SECTION("Distance with negative value") {
        auto intermediate_point = point_along_path(p1, p2, -5.0f);
        REQUIRE(intermediate_point.x == 0.0f);
        REQUIRE(intermediate_point.y == 0.0f);
    }

    SECTION("Point along path with coincident points") {
        auto intermediate_point = point_along_path(p1, p1, 5.0f);
        REQUIRE(intermediate_point.x == 0.0f);
        REQUIRE(intermediate_point.y == 0.0f);
    }
}