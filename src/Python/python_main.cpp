

#include <omp.h>
#include <pybind11/pybind11.h>


namespace py = pybind11;

void init_whisker_tracker(py::module &m);

PYBIND11_MODULE(whisker, m) {

    init_whisker_tracker(m);

    m.def("get_max_threads", &omp_get_max_threads);
    m.def("set_num_threads", &omp_set_num_threads);
};
