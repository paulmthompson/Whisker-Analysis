
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "whisker.hpp"

#include "test_constants.hpp"

TEST_CASE("CreateSet - 1", "[Set]") {

    auto v = std::vector<whisker::Point2D<int>>{p1_int, p2_int};

    auto s = whisker::create_set(v);

    REQUIRE(s.size() == 2);

}

TEST_CASE("CreateSet - 2", "[Set]") {

auto s = whisker::create_set(line100);

REQUIRE(s.size() == 2);

}

TEST_CASE("CreateSet - 3", "[Set]") {

auto s = whisker::create_set(line100_2);

REQUIRE(s.size() == 101);

}

TEST_CASE("CreateSet - 4", "[Set]") {

    auto s = whisker::create_set(line100_2);

    auto s2 = whisker::create_set(line100_3);

    std::vector<whisker::Point2D<int>> v_intersection;
    std::set_intersection(s.begin(), s.end(), s2.begin(), s2.end(),
        std::back_inserter(v_intersection));

    REQUIRE(v_intersection.size() == 51);
}