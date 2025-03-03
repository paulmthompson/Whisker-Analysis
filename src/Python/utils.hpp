#include "Geometry/lines.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

inline whisker::Line2D convert_np_array_to_line2d(const py::array_t<float>& array) {
    whisker::Line2D line;
    auto buf = array.request();
    if (buf.ndim != 2 || buf.shape[1] != 2) {
        throw std::runtime_error("Input should be a 2D numpy array with shape (n, 2)");
    }

    float* ptr = static_cast<float*>(buf.ptr);
    for (std::size_t i = 0; i < buf.shape[0]; ++i) {
        line.push_back(whisker::Point2D<float>{ptr[i * 2], ptr[i * 2 + 1]});
    }

    return line;
};