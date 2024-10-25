

#include "Geometry/lines.hpp"
#include "Geometry/points.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <string>

#include "test_constants.hpp"

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

TEST_CASE("LengthTest - 4", "[Distance]") {

REQUIRE(whisker::length(line100) == 1.0f);

BENCHMARK("length Calculation") {
    return whisker::length(line100);
};
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

TEST_CASE("Nearest Preceding - 3", "[Distance]") {

auto [index, dist] = nearest_preceding_index_along_path(line100, 0.905f);

REQUIRE(index == 90);
REQUIRE(dist == 0.9f);

BENCHMARK("Nearest Preceding Calculation") {
    return nearest_preceding_index_along_path(line100, 0.905f);
};
}

TEST_CASE("Nearest Interpolated - 1", "[Distance]") {

auto line = whisker::Line2D{p1, p2, p3};

auto intermediate_point = point_at_pathlength(line, 15.0);

REQUIRE(intermediate_point.x == 5.0f);
REQUIRE(intermediate_point.y == 10.0f);
}

TEST_CASE("Nearest Interpolated - 2", "[Distance]") {

auto intermediate_point = point_at_pathlength(line100, 0.905f);

REQUIRE(intermediate_point.x == 0.0f);
REQUIRE(intermediate_point.y == 0.905f);

BENCHMARK("Nearest Interpolated Calculation") {
return point_at_pathlength(line100, 0.905f);
};
}