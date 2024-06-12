
#ifndef WHISKER_ANALYSIS_TEST_CONSTANTS_HPP
#define WHISKER_ANALYSIS_TEST_CONSTANTS_HPP

#include "whisker.hpp"

inline auto p1 = whisker::Point2D{0.0f, 0.0f};
inline auto p2 = whisker::Point2D{0.0f, 10.0f};
inline auto p3 = whisker::Point2D{10.0f, 10.0f};
inline auto p4 = whisker::Point2D{6.0f, 8.0f};

inline auto p1_int = whisker::Point2D<int>{0, 0};
inline auto p2_int = whisker::Point2D<int>{1, 1};

inline auto create_line = [](float scaling){
    auto line = whisker::Line2D();
    for (int i = 0; i < 101; i ++) {
        line.push_back(whisker::Point2D<float>{0.0f, i / scaling});
    }
    return line;
};

inline auto line100 = create_line(100.0f);

inline auto line100_2 = create_line(1.0f);

inline auto line100_3 = create_line(2.0f);

#endif //WHISKER_ANALYSIS_TEST_CONSTANTS_HPP
