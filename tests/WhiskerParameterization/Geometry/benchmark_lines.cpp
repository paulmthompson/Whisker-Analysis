

#include "Geometry/lines.hpp"
#include "Geometry/points.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <string>

#include "../test_constants.hpp"


TEST_CASE("Benchmark Length Calculation ", "[BENCHMARK]") {


BENCHMARK("length Calculation") {
    return whisker::length(line100);
};
}

TEST_CASE("Benchmark Nearest Preceding", "[BENCHMARK]") {

BENCHMARK("Nearest Preceding Calculation") {
    return nearest_preceding_index_along_path(line100, 0.905f);
};
}


TEST_CASE("Benchmark Nearest Interpolated", "[BENCHMARK]") {

BENCHMARK("Nearest Interpolated Calculation") {
return point_at_pathlength(line100, 0.905f);
};
}