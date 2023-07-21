#include "../common.hxx"
#include <nanobind/stl/array.h>


static unsigned int global_array [10];


NANOBIND_MODULE(array, m) {
    m.def_buffer([](unsigned int &array);
}

