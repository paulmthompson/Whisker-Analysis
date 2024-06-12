#ifndef WHISKER_ANALYSIS_WHISKER_HPP
#define WHISKER_ANALYSIS_WHISKER_HPP

namespace whisker {

template<typename T>
struct Point2D<T> {
    T x;
    T y;
};

float distance(Point2D<float> p1, Point2D<float> p2);

}
#endif //WHISKER_ANALYSIS_WHISKER_HPP
