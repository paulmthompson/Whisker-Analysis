
#include "test_data.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <iostream>


TEST_CASE("Test Trace", "[Trace]") {

    static auto test_img = load_img("data/test_img.bin");
    static auto test_img2 = load_img("data/test_img2.bin");
    static auto test_img3 = load_img("data/test_img3.bin");
    static auto wt = create_warmed_up_wt(test_img);

    REQUIRE(test_img.size() == img_height * img_width);

    wt.trace(test_img, img_height, img_width);

    REQUIRE(wt.whiskers.size() == 5);

    wt.setWhiskerPad(576.0f, 273.0f);
    wt.trace(test_img2, img_height, img_width);

    REQUIRE(wt.whiskers.size() == 5);

    wt.trace(test_img3, img_height, img_width);
    REQUIRE(wt.whiskers.size() == 5);

}

