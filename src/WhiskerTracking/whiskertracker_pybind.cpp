#include "whiskertracker.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <iostream>

namespace py = pybind11;

PYBIND11_MODULE(whiskertracker, m) {
    py::class_<whisker::WhiskerTracker>(m, "WhiskerTracker")
            .def(py::init<>())
            .def("setWhiskerLengthThreshold", &whisker::WhiskerTracker::setWhiskerLengthThreshold)
            .def("getWhiskerPadRadius", &whisker::WhiskerTracker::getWhiskerPadRadius)
            .def("setWhiskerPadRadius", &whisker::WhiskerTracker::setWhiskerPadRadius)
            .def("getWhiskerPad", [](whisker::WhiskerTracker& wt) {
                auto pad = wt.getWhiskerPad();
                return py::make_tuple(pad.x, pad.y);
            })
            .def("setWhiskerPad", &whisker::WhiskerTracker::setWhiskerPad)
            .def("trace", [](whisker::WhiskerTracker &wt, py::array_t<uint8_t, py::array::c_style | py::array::forcecast> image, int image_height, int image_width) {
                py::buffer_info info = image.request();

                if (info.ndim != 1) {
                    throw std::runtime_error("Number of dimensions must be one");
                }

                if (info.format != py::format_descriptor<uint8_t>::format()) {
                    throw std::runtime_error("Image must be of type uint8_t");
                }

                if (info.size != image_height * image_width) {
                    throw std::runtime_error("Image size must match image height and width");
                }

                auto ptr = static_cast<uint8_t *>(info.ptr);
                std::vector<uint8_t> image_data(ptr, ptr + info.size);
                auto whiskers = wt.trace(image_data, image_height, image_width);

                std::vector<py::array_t<float>> whiskers_py;
                for (const auto &line : whiskers) {
                    std::vector<float> line_py;
                    for (const auto &point : line) {
                        line_py.push_back(point.x);
                        line_py.push_back(point.y);
                    }
                    whiskers_py.push_back(py::array(line_py.size(), line_py.data()));
                }
                return whiskers_py;
            })
            .def("setFaceMask", [](whisker::WhiskerTracker &wt, py::list mask) {
                std::vector<whisker::Point2D<float>> mask_cpp;
                for (py::handle obj : mask) {
                    py::tuple t = obj.cast<py::tuple>();
                    mask_cpp.push_back(whisker::Point2D<float>{t[0].cast<float>(), t[1].cast<float>()});
                }
                wt.setFaceMask(mask_cpp);
            });
};
