
#include "Geometry/lines.hpp"
#include "Geometry/points.hpp"
#include "loaders.hpp"
#include "Python/utils.hpp"
#include "whiskertracker.hpp"

#include <omp.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <chrono>
#include <cstddef>
#include <iostream>

namespace py = pybind11;

void init_whisker_tracker(py::module &m) {

    auto whisker_tracker = m.def_submodule("WhiskerTracker");
    py::class_<whisker::WhiskerTracker>(whisker_tracker, "WhiskerTracker")
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
            .def("trace_multiple_images", [](whisker::WhiskerTracker &wt, py::list images_list, int image_height, int image_width) {
                std::vector<std::vector<uint8_t>> images_cpp;
                for (py::handle image_py : images_list) {
                    py::array_t<uint8_t, py::array::c_style | py::array::forcecast> image = image_py.cast<py::array_t<uint8_t>>();
                    py::buffer_info info = image.request();
                    if (info.ndim != 1 || info.format != py::format_descriptor<uint8_t>::format() || info.size != image_height * image_width) {
                        throw std::runtime_error("Each image must be a 1D uint8_t array with size matching image_height * image_width");
                    }
                    auto ptr = static_cast<uint8_t *>(info.ptr);
                    images_cpp.push_back(std::vector<uint8_t>(ptr, ptr + info.size));
                }
                auto whiskers = wt.trace_multiple_images(images_cpp, image_height, image_width);

                py::list whiskers_py;
                for (const auto &image_whiskers : whiskers) {
                    py::list image_whiskers_py;
                    for (const auto &line : image_whiskers) {
                        py::list line_py;
                        for (const auto &point : line) {
                            line_py.append(py::make_tuple(point.x, point.y));
                        }
                        image_whiskers_py.append(line_py);
                    }
                    whiskers_py.append(image_whiskers_py);
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
            })
            .def("load_and_align_whiskers", [](whisker::WhiskerTracker &wt, const std::string &filepath) {
                auto data = whisker::load_line_csv(filepath, true);
                std::cout << "Loaded " << data.size() << " frames from " << filepath << std::endl;

                auto t1 = std::chrono::high_resolution_clock::now();
                const auto whisker_pad = wt.getWhiskerPad();
                
                for (auto & [frame_num, lines] : data) {
                    for (auto & line : lines) {
                        whisker::align_whisker_to_follicle(line, whisker_pad);
                    }
                }
                auto t2 = std::chrono::high_resolution_clock::now();

                auto duration = std::chrono::duration<double>(t2 - t1).count();
                std::cout << "Aligned " << data.size() << " frames in " << duration << "s" << std::endl;

                std::string output_filepath = filepath.substr(0, filepath.find_last_of('.')) + "_aligned.csv";
                whisker::save_lines_csv(data,output_filepath);
            })
            .def("frechet_distance", [](whisker::WhiskerTracker &wt, const py::array_t<float>& whisker1, const py::array_t<float>& whisker2) {
                
                whisker::Line2D line1 = convert_np_array_to_line2d(whisker1);
                whisker::Line2D line2 = convert_np_array_to_line2d(whisker2);
                return whisker::fast_discrete_frechet_matrix(line1, line2);
            });
}
