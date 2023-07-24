#pragma once

#ifdef ENABLE_NB
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
namespace py = nanobind;
#define DEFINE_MODULE NB_MODULE
#define MODULE_NAME(name) name ## _nb
inline std::string obj2str (const py::object &obj) {
    return py::repr(obj).c_str(); }

#elif ENABLE_PB
#include <pybind11/pybind11.h>
namespace py = pybind11;
#define DEFINE_MODULE PYBIND11_MODULE
#define MODULE_NAME(name) name ## _pb
inline std::string obj2str (const py::object &obj) {
    return py::repr(obj).cast<std::string>(); }

#else
#error "No macro ENABLE_NB nor ENABLE_PB defined"
#endif

//#include <pybind11/pybind11.h>
//pybind11::repr(1).cast<std::string>();
//#include <nanobind/nanobind.h>
//nanobind::repr(1).c_str()
