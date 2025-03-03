
#include "Geometry/lines.hpp"
#include "whiskertracker.hpp"
#include "loaders.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Test CSV Load", "[Load]") {

    auto whiskers = whisker::load_line_csv("data/whiskers.csv");

    REQUIRE(whiskers.size() == 168);

}
