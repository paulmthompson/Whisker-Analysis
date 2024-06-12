
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