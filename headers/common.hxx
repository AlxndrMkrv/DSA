#pragma once

#include <sstream>

#ifdef ENABLE_NB
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/array.h>
#include <nanobind/operators.h>
namespace py = nanobind;

#define DEFINE_MODULE NB_MODULE
#define MODULE_NAME(name) name ## _nb

#elif ENABLE_PB
#include <pybind11/pybind11.h>
namespace py = pybind11;

#define DEFINE_MODULE PYBIND11_MODULE
#define MODULE_NAME(name) name ## _pb

namespace pybind11 { using rv_policy = pybind11::return_value_policy; }

#else
#error "No macro ENABLE_NB nor ENABLE_PB defined"
#endif

inline size_t id (const py::object & obj) {
    return reinterpret_cast<size_t>(obj.ptr()); }

inline std::string obj2str (const py::object & obj) {
#ifdef ENABLE_NB
    return py::repr(obj).c_str();
#elif ENABLE_PB
    return py::repr(obj).cast<std::string>();
#endif
}

inline py::object itr2obj (const py::iterator & itr) {
#ifdef ENABLE_NB
    return py::borrow(*itr);
#elif ENABLE_PB
    return py::reinterpret_borrow<py::object>(*itr);
#endif
}
