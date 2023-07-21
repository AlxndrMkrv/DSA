cmake_minimum_required(VERSION 3.12)

project(DSA LANGUAGES CXX)

find_package(Python COMPONENTS Interpreter
                               Development.Module
                    REQUIRED
)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug"
                                                       "Release"
                                                       "MinSizeRel"
                                                       "RelWithDebInfo")
endif()

# Detect the installed nanobind package and import it into CMake
execute_process(COMMAND ${Python_EXECUTABLE} -m nanobind --cmake_dir
                OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
find_package(nanobind CONFIG REQUIRED)

# Detect the installed pybind11 package and import it into CMake
execute_process(COMMAND ${Python_EXECUTABLE} -m pybind11 --cmake_dir
                OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE PB_DIR)
list(APPEND CMAKE_PREFIX_PATH ${PB_DIR})
find_package(pybind11 CONFIG REQUIRED)

include(array/module.cmake)
include(conversion/module.cmake)
include(list/module.cmake)


