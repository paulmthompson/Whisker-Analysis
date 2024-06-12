
#include <catch2/catch_test_macros.hpp>
#include "whisker.hpp"

auto p1 = whisker::Point2D{0.0f, 0.0f};
auto p2 = whisker::Point2D{0.0f, 10.0f};
auto p3 = whisker::Point2D{10.0f, 10.0f};
auto p4 = whisker::Point2D{6.0f, 8.0f};

TEST_CASE("DistanceTest - 1", "[Distance]") {

    REQUIRE(whisker::distance(p1,p2) == 10.0f);
}

TEST_CASE("DistanceTest - 2", "[Distance]") {

REQUIRE_FALSE(whisker::distance(p1,p2) == 9.0f);
}

TEST_CASE("LengthTest - 1", "[Distance]") {

auto line = whisker::Line2D{p1, p2};

REQUIRE(whisker::length(line) == 10.0f);
}

TEST_CASE("LengthTest - 2", "[Distance]") {

auto line = whisker::Line2D{p1};

REQUIRE(whisker::length(line) == 0.0f);
}

TEST_CASE("LengthTest - 3", "[Distance]") {

auto line = whisker::Line2D{p1, p2, p3};

REQUIRE(whisker::length(line) == 20.0f);
}

TEST_CASE("Intermediate Point - 1", "[Distance]") {

auto intermediate_point = point_along_path(p1, p2, 5.0f);

REQUIRE(intermediate_point.x == 0.0f);
REQUIRE(intermediate_point.y == 5.0f);
}

TEST_CASE("Intermediate Point - 2", "[Distance]") {

auto intermediate_point = point_along_path(p1, p2, 15.0f);

REQUIRE(intermediate_point.x == 0.0f);
REQUIRE(intermediate_point.y == 0.0f);
}

TEST_CASE("Intermediate Point - 3", "[Distance]") {


auto intermediate_point = point_along_path(p1, p4, 5.0f);

REQUIRE(intermediate_point.x == 3.0f);
REQUIRE(intermediate_point.y == 4.0f);
}

TEST_CASE("Nearest Preceding - 1", "[Distance]") {

auto line = whisker::Line2D{p1, p2, p3};

auto [index, dist] = nearest_preceding_index_along_path(line, 5.0);

REQUIRE(index == 0);
REQUIRE(dist == 0.0f);
}

TEST_CASE("Nearest Preceding - 2", "[Distance]") {

auto line = whisker::Line2D{p1, p2, p3};

auto [index, dist] = nearest_preceding_index_along_path(line, 11.0);

REQUIRE(index == 1);
REQUIRE(dist == 10.0f);
}

TEST_CASE("Nearest Interpolated - 1", "[Distance]") {

auto line = whisker::Line2D{p1, p2, p3};

auto intermediate_point = point_at_pathlength(line, 15.0);

REQUIRE(intermediate_point.x == 5.0f);
REQUIRE(intermediate_point.y == 10.0f);
}