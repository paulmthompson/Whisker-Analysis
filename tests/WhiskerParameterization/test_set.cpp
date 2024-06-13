
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "whisker.hpp"

#include <iostream>

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

TEST_CASE("IoU - 1", "[Set]") {

    auto IoU = calculate_overlap_iou(line100_2, line100_3);

    std::cout << "IoU calculated as " << IoU << std::endl;

    REQUIRE(IoU > 0.2);

    BENCHMARK("IoU Calculation") {
        return calculate_overlap_iou(line100_2, line100_3);
    };
}

TEST_CASE("IoU - 2", "[Set]") {

    auto IoU = calculate_overlap_iou_relative(line100_2, line100_3);

    std::cout << "IoU calculated as " << IoU << std::endl;

    REQUIRE(IoU == 1.0);

    BENCHMARK("IoU Calculation") {
        return calculate_overlap_iou_relative(line100_2, line100_3);
    };
}

TEST_CASE("Mask Intersection - 1", "[Set]") {

    auto mask = whisker::Mask2D{whisker::Point2D<int>{0, 0}};
    auto intersection = intersect(line100[0],mask);

    REQUIRE(intersection == true);

}

TEST_CASE("Mask Intersection - 2", "[Set]") {

    auto mask = whisker::Mask2D{whisker::Point2D<int>{5, 5},whisker::Point2D<int>{4,4}};
    auto intersection = intersect(line100[0],mask);

    REQUIRE(intersection == false);

}