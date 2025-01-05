
#include "Geometry/lines.hpp"
#include "whiskertracker.hpp"
#include "loaders.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

TEST_CASE("Test CSV Load", "[Load]") {

    auto whiskers = whisker::load_line_csv("data/whiskers.csv");

    CHECK(whiskers.size() == 168);

}
