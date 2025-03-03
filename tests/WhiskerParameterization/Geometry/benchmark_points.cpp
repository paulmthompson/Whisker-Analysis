
#include "Geometry/points.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <string>

#include "../test_constants.hpp"

TEST_CASE("Benchmark Distance Between Points", "[BENCHMARK]") {

    BENCHMARK("Distance Calculation") {
        return whisker::distance(p1,p2);
    };
}
