
#include "test_data.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <iostream>


TEST_CASE("Test Trace", "[Trace]") {

    static auto wt = create_warmed_up_wt();
    static auto test_img = load_img("data/test_img.bin");

    std::ifstream stream("data/test_img.bin", std::ios::in | std::ios::binary);

    std::vector<uint8_t> buffer(img_height * img_width);
    stream.read(reinterpret_cast<char*>(buffer.data()), img_height * img_width);
    stream.close();

    REQUIRE(test_img.size() == img_height * img_width);

    std::cout << "The second index of test img is " << unsigned(test_img[1]) << std::endl;
std::cout << "The second index of test img is " << unsigned(buffer[1]) << std::endl;

    wt.trace(test_img, img_height, img_width);

    REQUIRE(wt.whiskers.size() == 6);

}

