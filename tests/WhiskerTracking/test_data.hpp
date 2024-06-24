#ifndef WHISKER_ANALYSIS_TEST_DATA_HPP
#define WHISKER_ANALYSIS_TEST_DATA_HPP

#include <fstream>
#include <string>
#include <vector>

#include "whiskertracker.hpp"

std::string data_filename = "data/test_img.bin";

inline const int img_width = 640;
inline const int img_height = 480;



inline auto load_img = [](std::string filename){
    std::ifstream stream(filename, std::ios::in | std::ios::binary);

    std::vector<uint8_t> buffer;
    buffer.resize(img_height * img_width);
    stream.read(reinterpret_cast<char*>(buffer.data()), img_height * img_width);
    stream.close();
    return buffer;
};


inline auto sample_img1 = load_img(data_filename);

inline auto wt = whisker::WhiskerTracker();


#endif //WHISKER_ANALYSIS_TEST_DATA_HPP
