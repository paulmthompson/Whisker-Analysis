

#include "Geometry/lines.hpp"
#include "whiskertracker.hpp"
#include "loaders.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

TEST_CASE("Benchmark CSV Load", "[BENCHMARK]") {

    BENCHMARK("CSV Load") {
        return whisker::load_line_csv("../tests/WhiskerTracking/data/whiskers.csv");
    };

}

TEST_CASE("Benchmark CSV Write", "[BENCHMARK]")
{
    auto data = whisker::load_line_csv("../tests/WhiskerTracking/data/whiskers.csv");

    BENCHMARK("CSV Write") {
        whisker::save_lines_csv(data, "test.csv");
    };
}
