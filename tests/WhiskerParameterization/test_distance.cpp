
#include <catch2/catch_test_macros.hpp>
#include "whisker.hpp"

TEST_CASE("DistanceTest - 1", "[Distance]") {
    auto p1 = whisker::Point2D{0.0f, 0.0f};
    auto p2 = whisker::Point2D{0.0f, 10.0f};

    REQUIRE(whisker::distance(p1,p2) == 10.0f);
}

TEST_CASE("DistanceTest - 2", "[Distance]") {
auto p1 = whisker::Point2D{0.0f, 0.0f};
auto p2 = whisker::Point2D{0.0f, 10.0f};

REQUIRE_FALSE(whisker::distance(p1,p2) == 9.0f);
}

TEST_CASE("LengthTest - 1", "[Distance]") {
auto p1 = whisker::Point2D{0.0f, 0.0f};
auto p2 = whisker::Point2D{0.0f, 10.0f};

auto line = whisker::Line2D{p1, p2};

REQUIRE(whisker::length(line) == 10.0f);
}

TEST_CASE("LengthTest - 2", "[Distance]") {
auto p1 = whisker::Point2D{0.0f, 0.0f};

auto line = whisker::Line2D{p1};

REQUIRE(whisker::length(line) == 0.0f);
}

TEST_CASE("LengthTest - 3", "[Distance]") {
auto p1 = whisker::Point2D{0.0f, 0.0f};
auto p2 = whisker::Point2D{0.0f, 10.0f};
auto p3 = whisker::Point2D{10.0f, 10.0f};

auto line = whisker::Line2D{p1, p2, p3};

REQUIRE(whisker::length(line) == 20.0f);
}

TEST_CASE("Intermediate Point - 1", "[Distance]") {
auto p1 = whisker::Point2D{0.0f, 0.0f};
auto p2 = whisker::Point2D{0.0f, 10.0f};

auto p3 = point_along_path(p1, p2, 5.0f);

REQUIRE(p3.x == 0.0f);
REQUIRE(p3.y == 5.0f);
}

TEST_CASE("Intermediate Point - 2", "[Distance]") {
auto p1 = whisker::Point2D{0.0f, 0.0f};
auto p2 = whisker::Point2D{0.0f, 10.0f};

auto p3 = point_along_path(p1, p2, 15.0f);

REQUIRE(p3.x == 0.0f);
REQUIRE(p3.y == 0.0f);
}

TEST_CASE("Intermediate Point - 3", "[Distance]") {
auto p1 = whisker::Point2D{0.0f, 0.0f};
auto p2 = whisker::Point2D{6.0f, 8.0f};

auto p3 = point_along_path(p1, p2, 5.0f);

REQUIRE(p3.x == 3.0f);
REQUIRE(p3.y == 4.0f);
}

TEST_CASE("Nearest Preceding - 1", "[Distance]") {
auto p1 = whisker::Point2D{0.0f, 0.0f};
auto p2 = whisker::Point2D{0.0f, 10.0f};
auto p3 = whisker::Point2D{10.0f, 10.0f};

auto line = whisker::Line2D{p1, p2, p3};

auto [index, dist] = nearest_preceding_index_along_path(line, 5.0);

REQUIRE(index == 0);
REQUIRE(dist == 0.0f);
}

TEST_CASE("Nearest Preceding - 2", "[Distance]") {
auto p1 = whisker::Point2D{0.0f, 0.0f};
auto p2 = whisker::Point2D{0.0f, 10.0f};
auto p3 = whisker::Point2D{10.0f, 10.0f};

auto line = whisker::Line2D{p1, p2, p3};

auto [index, dist] = nearest_preceding_index_along_path(line, 11.0);

REQUIRE(index == 1);
REQUIRE(dist == 10.0f);
}