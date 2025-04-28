

#include "Geometry/lines.hpp"
#include "Geometry/points.hpp"

#include <catch2/catch_test_macros.hpp>

inline auto create_line = [](float scaling){
    auto line = whisker::Line2D();
    for (int i = 0; i < 101; i ++) {
        line.push_back(whisker::Point2D<float>{0.0f, i / scaling});
    }
    return line;
};

TEST_CASE("Line length calculations", "[lines][distance]") {

    auto p1 = whisker::Point2D<float>{0.0f, 0.0f};
    auto p2 = whisker::Point2D<float>{0.0f, 10.0f};
    auto p3 = whisker::Point2D<float>{10.0f, 10.0f};

    auto line100 = create_line(100.0f);

    SECTION("Length of line with two points") {
        auto line = whisker::Line2D{p1, p2};
        REQUIRE(whisker::length(line) == 10.0f);
    }

    SECTION("Length of line with three points") {
        auto line = whisker::Line2D{p1, p2, p3};
        REQUIRE(whisker::length(line) == 20.0f);
    }

    SECTION("Length of densely sampled line") {
        REQUIRE(whisker::length(line100) == 1.0f);
    }
}

TEST_CASE("Finding points along paths", "[lines][interpolation]") {

    auto p1 = whisker::Point2D<float>{0.0f, 0.0f};
    auto p2 = whisker::Point2D<float>{0.0f, 10.0f};
    auto p3 = whisker::Point2D<float>{10.0f, 10.0f};

    auto line100 = create_line(100.0f);

    SECTION("Find point at specific path length") {
        auto line = whisker::Line2D{p1, p2, p3};
        auto intermediate_point = point_at_pathlength(line, 15.0);
        REQUIRE(intermediate_point.x == 5.0f);
        REQUIRE(intermediate_point.y == 10.0f);
    }

    SECTION("Find point at path length in dense line") {
        auto intermediate_point = point_at_pathlength(line100, 0.905f);
        REQUIRE(intermediate_point.x == 0.0f);
        REQUIRE(intermediate_point.y == 0.905f);
    }
}

TEST_CASE("Finding nearest preceding point indices", "[lines][search]") {

    auto p1 = whisker::Point2D<float>{0.0f, 0.0f};
    auto p2 = whisker::Point2D<float>{0.0f, 10.0f};
    auto p3 = whisker::Point2D<float>{10.0f, 10.0f};

    auto line100 = create_line(100.0f);

    SECTION("Find index before path distance") {
        auto line = whisker::Line2D{p1, p2, p3};
        auto [index, dist] = nearest_preceding_index_along_path(line, 5.0);
        REQUIRE(index == 0);
        REQUIRE(dist == 0.0f);
    }

    SECTION("Find index at segment boundary") {
        auto line = whisker::Line2D{p1, p2, p3};
        auto [index, dist] = nearest_preceding_index_along_path(line, 11.0);
        REQUIRE(index == 1);
        REQUIRE(dist == 10.0f);
    }

    SECTION("Find index in dense line") {
        auto [index, dist] = nearest_preceding_index_along_path(line100, 0.905f);
        REQUIRE(index == 90);
        REQUIRE(dist == 0.9f);
    }
}

TEST_CASE("Edge cases for line operations", "[lines][edge_cases]") {

    auto p1 = whisker::Point2D<float>{0.0f, 0.0f};

    SECTION("Length of single point line") {
        auto line = whisker::Line2D{p1};
        REQUIRE(whisker::length(line) == 0.0f);
    }
}