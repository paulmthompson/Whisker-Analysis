#ifndef WHISKER_ANALYSIS_GEOMETRY_HPP
#define WHISKER_ANALYSIS_GEOMETRY_HPP

namespace whisker {

template <typename T>
struct Point2D {
    T x;
    T y;

    friend bool operator< (Point2D<T> const & p1, Point2D<T> const & p2)
    {
        if (p1.x != p2.x) {
            return p1.x < p2.x;
        } else {
            return p1.y < p2.y;
        }
    }
};


}

#endif //WHISKER_ANALYSIS_GEOMETRY_HPP
