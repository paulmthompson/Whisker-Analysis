#ifndef WHISKER_ANALYSIS_TEST_DATA_HPP
#define WHISKER_ANALYSIS_TEST_DATA_HPP

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "whiskertracker.hpp"

std::string data_filename = "data/test_img.bin";

inline const int img_width = 640;
inline const int img_height = 480;

inline auto load_img = [](std::string filename){
    std::ifstream stream(filename, std::ios::in | std::ios::binary);

    std::vector<uint8_t> buffer(img_height * img_width);
    stream.read(reinterpret_cast<char*>(buffer.data()), img_height * img_width);
    stream.close();
    return buffer;
};

inline auto sample_img1 = load_img(data_filename);

inline auto create_warmed_up_wt = []() {
    auto wt = whisker::WhiskerTracker();
    wt.setWhiskerPad(584.0f, 288.0f);

    wt.trace(sample_img1, img_height, img_width);
    return wt;
};


#endif //WHISKER_ANALYSIS_TEST_DATA_HPP
