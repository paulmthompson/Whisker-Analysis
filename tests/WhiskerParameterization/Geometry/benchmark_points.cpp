
#include "Geometry/points.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

TEST_CASE("Benchmark Distance Between Points", "[BENCHMARK]") {

    auto p1 = whisker::Point2D<float>{0.0f, 0.0f};
    auto p2 = whisker::Point2D<float>{0.0f, 10.0f};

    BENCHMARK("Distance Calculation") {
        return whisker::distance(p1,p2);
    };
}
