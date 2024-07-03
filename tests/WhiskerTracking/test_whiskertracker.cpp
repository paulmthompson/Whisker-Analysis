
#include "Geometry/lines.hpp"
#include "whiskertracker.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "test_data.hpp"

TEST_CASE("Test Trace", "[Trace]") {

    static auto test_img = load_img("data/0110_1_0000000.bin");
    static auto wt = create_warmed_up_wt(test_img);

    wt.setWhiskerPad(577.0f, 202.0f);
    wt.trace(test_img, img_height, img_width);

    CHECK(wt.whiskers.size() == 5);

}

