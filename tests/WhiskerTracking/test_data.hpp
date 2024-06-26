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
    std::ifstream stream(filename, std::ios::binary);

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    return data;
};

inline auto create_warmed_up_wt = [](std::vector<uint8_t> & img) {
    auto wt = whisker::WhiskerTracker();
    wt.setWhiskerPad(582.0f, 267.0f);

    wt.trace(img, img_height, img_width);
    return wt;
};


#endif //WHISKER_ANALYSIS_TEST_DATA_HPP
