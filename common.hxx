#pragma once

#ifdef ENABLE_NB
#include <nanobind/nanobind.h>
namespace py = nanobind;
const auto index_error = nanobind::index_error;
#define DEFINE_MODULE NB_MODULE
#define MODULE_NAME(name) name ## _nb

#elif ENABLE_PB
#include <pybind11/pybind11.h>
namespace py = pybind11;
using index_error = pybind11::index_error;
#define DEFINE_MODULE PYBIND11_MODULE
#define MODULE_NAME(name) name ## _pb

#else
#error "No macro ENABLE_NB nor ENABLE_PB defined"
#endif

