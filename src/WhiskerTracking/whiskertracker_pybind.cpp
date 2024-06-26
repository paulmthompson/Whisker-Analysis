#include "whiskertracker.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>


namespace py = pybind11;

PYBIND11_MODULE(whiskertracker, m) {
    py::class_<whisker::WhiskerTracker>(m, "WhiskerTracker")
            .def(py::init<>())
            .def("setWhiskerLengthThreshold", &whisker::WhiskerTracker::setWhiskerLengthThreshold)
            .def("getWhiskerPadRadius", &whisker::WhiskerTracker::getWhiskerPadRadius)
            .def("setWhiskerPadRadius", &whisker::WhiskerTracker::setWhiskerPadRadius)
            .def("getWhiskerPad", &whisker::WhiskerTracker::getWhiskerPad)
            .def("setWhiskerPad", &whisker::WhiskerTracker::setWhiskerPad)
            .def("trace", [](whisker::WhiskerTracker &wt, py::array_t<uint8_t> image, int image_height, int image_width) {
                py::buffer_info info = image.request();

                auto ptr = static_cast<uint8_t *>(info.ptr);
                std::vector<uint8_t> image_data(ptr, ptr + info.size);
                wt.trace(image_data, image_height, image_width);
            })
            .def("getWhiskers", [](whisker::WhiskerTracker &wt) {
                std::vector<py::array_t<float>> whiskers_py;
                for (const auto &line : wt.whiskers) {
                    std::vector<float> line_py;
                    for (const auto &point : line) {
                        line_py.push_back(point.x);
                        line_py.push_back(point.y);
                    }
                    whiskers_py.push_back(py::array(line_py.size(), line_py.data()));
                }
                return whiskers_py;
            });
};
