
#include "Geometry/lines.hpp"
#include "whiskertracker.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "test_data.hpp"

TEST_CASE("Test Trace", "[Trace]") {

    static auto test_img = load_img("data/0110_1_0000000.bin");
    static auto wt = create_warmed_up_wt(test_img);

    wt.setWhiskerPad(577.0f, 202.0f);
    auto whiskers = wt.trace(test_img, img_height, img_width);

    CHECK(whiskers.size() == 5);

}

TEST_CASE("Test Trace Multi", "[Trace]") {

    static auto test_img = load_img("data/0110_1_0000000.bin");
    static auto test_img2 = load_img("data/test_img.bin");
    static auto wt = create_warmed_up_wt(test_img);

    wt.setWhiskerPad(577.0f, 202.0f);

    std::vector<std::vector<uint8_t>> images = {test_img};

    auto whiskers = wt.trace_multiple_images(images, img_height, img_width);

    CHECK(whiskers.size() == 1);
    CHECK(whiskers[0].size() == 5);

    std::vector<std::vector<uint8_t>> images2{
        load_img("data/0110_1_0000000.bin"),
        load_img("data/test_img.bin")};

    CHECK(images2.size() == 2);

    auto whiskers2 = wt.trace_multiple_images(images2, img_height, img_width);

    CHECK(whiskers2.size() == 2);
    CHECK(whiskers2[0].size() == 5);

    whiskers2 = wt.trace_multiple_images(images2, img_height, img_width);

    CHECK(whiskers2.size() == 2);
    CHECK(whiskers2[0].size() == 5);

}

