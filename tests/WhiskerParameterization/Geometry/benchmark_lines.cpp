

#include "Geometry/lines.hpp"
#include "Geometry/points.hpp"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

inline auto create_line = [](float scaling) {
    auto line = whisker::Line2D();
    for (int i = 0; i < 101; i++) {
        line.push_back(whisker::Point2D<float>{0.0f, i / scaling});
    }
    return line;
};

TEST_CASE("Benchmark Length Calculation ", "[BENCHMARK]") {

    auto line100 = create_line(100.0f);

    BENCHMARK("length Calculation") {
        return whisker::length(line100);
    };
}

TEST_CASE("Benchmark Nearest Preceding", "[BENCHMARK]") {

    auto line100 = create_line(100.0f);

    BENCHMARK("Nearest Preceding Calculation") {
        return nearest_preceding_index_along_path(line100, 0.905f);
    };
}


TEST_CASE("Benchmark Nearest Interpolated", "[BENCHMARK]") {

    auto line100 = create_line(100.0f);

    BENCHMARK("Nearest Interpolated Calculation") {
        return point_at_pathlength(line100, 0.905f);
    };
}