#include "whiskertracker.hpp"
#include "loaders.hpp"
#include "Geometry/lines.hpp"
#include "Geometry/points.hpp"

#include <omp.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <iostream>

namespace py = pybind11;

whisker::Line2D convert_np_array_to_line2d(const py::array_t<float>& array) {
    whisker::Line2D line;
    auto buf = array.request();
    if (buf.ndim != 2 || buf.shape[1] != 2) {
        throw std::runtime_error("Input should be a 2D numpy array with shape (n, 2)");
    }

    float* ptr = static_cast<float*>(buf.ptr);
    for (ssize_t i = 0; i < buf.shape[0]; ++i) {
        line.push_back(whisker::Point2D<float>{ptr[i * 2], ptr[i * 2 + 1]});
    }

    return line;
};

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
                auto data_map = whisker::load_line_csv(filepath);
                std::cout << "Loaded " << data_map.size() << " frames from " << filepath << std::endl;
                for (auto & [frame_num, lines] : data_map) {
                    for (auto & line : lines) {
                        whisker::align_whisker_to_follicle(line, wt.getWhiskerPad());
                    }
                }

                std::string output_filepath = filepath.substr(0, filepath.find_last_of('.')) + "_aligned.csv";
                whisker::save_lines_csv(data_map,output_filepath);
            })
            .def("frechet_distance", [](whisker::WhiskerTracker &wt, const py::array_t<float>& whisker1, const py::array_t<float>& whisker2) {
                
                whisker::Line2D line1 = convert_np_array_to_line2d(whisker1);
                whisker::Line2D line2 = convert_np_array_to_line2d(whisker2);
                return whisker::fast_discrete_frechet_matrix(line1, line2);
            });
    m.def("get_max_threads", &omp_get_max_threads);
    m.def("set_num_threads", &omp_set_num_threads);
};
