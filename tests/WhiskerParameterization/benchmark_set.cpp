
#include "Geometry/lines.hpp"
#include "Geometry/mask.hpp"
#include "Geometry/points.hpp"
#include "whisker.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>


#include "test_constants.hpp"


TEST_CASE("Benchmark IoU Calculation", "[BENCHMARK]") {

    BENCHMARK("IoU Calculation") {
        return calculate_overlap_iou(line100_2, line100_3);
    };
}

TEST_CASE("Benchmark Relative IoU Calculation", "[BENCHMARK]") {

    BENCHMARK("IoU Relative Calculation") {
        return calculate_overlap_iou_relative(line100_2, line100_3);
    };
}

TEST_CASE("Benchmark Mask Intersection", "[BENCHMARK]") {

    auto mask = whisker::Mask2D{whisker::Point2D<int>{0, 0}};

    BENCHMARK("Mask Intersection") {
        return intersect(line100[0],mask);
    };

}