
#include "Geometry/points.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>

#include "test_constants.hpp"

TEST_CASE("DistanceTest - 1", "[Distance]") {

    REQUIRE(whisker::distance(p1,p2) == 10.0f);

}

TEST_CASE("DistanceTest - 2", "[Distance]") {

REQUIRE_FALSE(whisker::distance(p1,p2) == 9.0f);
}

TEST_CASE("Intermediate Point - 1", "[Points]") {

auto intermediate_point = point_along_path(p1, p2, 5.0f);

REQUIRE(intermediate_point.x == 0.0f);
REQUIRE(intermediate_point.y == 5.0f);
}

TEST_CASE("Intermediate Point - 2", "[Points]") {

auto intermediate_point = point_along_path(p1, p2, 15.0f);

REQUIRE(intermediate_point.x == 0.0f);
REQUIRE(intermediate_point.y == 0.0f);
}

TEST_CASE("Intermediate Point - 3", "[Points]") {


auto intermediate_point = point_along_path(p1, p4, 5.0f);

REQUIRE(intermediate_point.x == 3.0f);
REQUIRE(intermediate_point.y == 4.0f);
}
