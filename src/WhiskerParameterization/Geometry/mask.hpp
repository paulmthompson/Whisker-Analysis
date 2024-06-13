#ifndef WHISKER_ANALYSIS_MASK_HPP
#define WHISKER_ANALYSIS_MASK_HPP

#include "Geometry/points.hpp"

#include <vector>

namespace whisker {

struct Mask2D : public std::vector<Point2D <int>> {
    using vector::vector;
};

template <typename T>
Mask2D create_mask(std::vector<Point2D<T>> vec) {
    auto mask = Mask2D();

    for (auto const & p : vec) {
        mask.push_back(Point2D<int>{static_cast<int>(p.x),static_cast<int>(p.y)});
    }
    return mask;
}

}
#endif //WHISKER_ANALYSIS_MASK_HPP
