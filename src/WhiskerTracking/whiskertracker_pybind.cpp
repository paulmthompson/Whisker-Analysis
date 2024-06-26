#include "whiskertracker.hpp"

#include <pybind11/pybind11.h>


namespace py = pybind11;

PYBIND11_MODULE(whiskertracker, m) {
    py::class_<whisker::WhiskerTracker>(m, "WhiskerTracker")
            .def(py::init<>())
            .def("setWhiskerLengthThreshold", &whisker::WhiskerTracker::setWhiskerLengthThreshold)
            .def("getWhiskerPadRadius", &whisker::WhiskerTracker::getWhiskerPadRadius)
            .def("setWhiskerPadRadius", &whisker::WhiskerTracker::setWhiskerPadRadius)
            .def("getWhiskerPad", &whisker::WhiskerTracker::getWhiskerPad);
};
/**
void init_whiskertracker(py::module &m) {
    py::class_<whisker::WhiskerTracker>(m, "WhiskerTracker")
            .def(py::init<>())
            .def("setWhiskerLengthThreshold", &whisker::WhiskerTracker::setWhiskerLengthThreshold)
            .def("getWhiskerPadRadius", &whisker::WhiskerTracker::getWhiskerPadRadius)
            .def("setWhiskerPadRadius", &whisker::WhiskerTracker::setWhiskerPadRadius)
            .def("getWhiskerPad", &whisker::WhiskerTracker::getWhiskerPad);
};
 **/