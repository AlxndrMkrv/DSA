#pragma once

#ifdef ENABLE_NB
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
namespace py = nanobind;
#define DEFINE_MODULE NB_MODULE
#define MODULE_NAME(name) name ## _nb
inline std::string obj2str (const py::object & obj) {
    return py::repr(obj).c_str(); }
inline py::object itr2obj (const py::iterator & itr) {
    return py::borrow(*itr);
}

#elif ENABLE_PB
#include <pybind11/pybind11.h>
namespace py = pybind11;
#define DEFINE_MODULE PYBIND11_MODULE
#define MODULE_NAME(name) name ## _pb
inline std::string obj2str (const py::object & obj) {
    return py::repr(obj).cast<std::string>(); }
inline py::object itr2obj (const py::iterator & itr) {
    return py::reinterpret_borrow<py::object>(*itr); }

#else
#error "No macro ENABLE_NB nor ENABLE_PB defined"
#endif
