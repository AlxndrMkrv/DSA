cmake_minimum_required(VERSION 3.12)

project(DSA LANGUAGES CXX)
enable_testing() # enable ctest

# Forbid to build project inside source dir
if (${CMAKE_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
    message(CRITICAL "Building the project inside source dir is prohibited!")
endif()

# Check if required python version provided
if(REQUIRED_PYTHON_VERSION)
    message(STATUS "Required Python version is ${REQUIRED_PYTHON_VERSION}")
else()
    message(STATUS "Python version will be detected automaticaly. "
                   "Set REQUIRED_PYTHON_VERSION if you need something specific"
    )
endif()

# Find python interpreter and includes/library
find_package(Python ${REQUIRED_PYTHON_VERSION}
                    COMPONENTS Interpreter Development
                    REQUIRED
)
set(Python_LIBRARY "python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}")

# Set build type if not defined outside
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug"
                                                         "Release"
                                                         "MinSizeRel"
                                                         "RelWithDebInfo"
    )
endif()

# Create symlink to "basic_performance.py" inside build directory to make it
# visible for *_performance targets
file(CREATE_LINK ${PROJECT_SOURCE_DIR}/py/basic_performance.py
                 ${PROJECT_BINARY_DIR}/basic_performance.py SYMBOLIC)

# Detect the installed nanobind package and import it into CMake
execute_process(COMMAND ${Python_EXECUTABLE} -m nanobind --cmake_dir
                OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR)
list(APPEND CMAKE_PREFIX_PATH ${NB_DIR})
find_package(nanobind CONFIG REQUIRED)

# Detect the installed pybind11 package and import it into CMake
execute_process(COMMAND ${Python_EXECUTABLE} -m pybind11 --cmakedir
                OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE PB_DIR)
list(APPEND CMAKE_PREFIX_PATH ${PB_DIR})
find_package(pybind11 CONFIG REQUIRED)

# Build nanobind static library
nanobind_build_library(nanobind-static)

# Common function to make library, performance and test targets
function(add_module)
    set(vars TARGET TEST PERFORMANCE DEBUG)
    set(lists SOURCES HEADERS)
    cmake_parse_arguments( P "" "${vars}" "${lists}" ${ARGN} )

    foreach(SRC ${P_SOURCES})
        list(APPEND MOD_SOURCES ${PROJECT_SOURCE_DIR}/${SRC})
    endforeach()

    set(PB_SONAME "${P_TARGET}_pb${CMAKE_SHARED_MODULE_SUFFIX}")
    set(NB_SONAME "${P_TARGET}_nb${CMAKE_SHARED_MODULE_SUFFIX}")

    # see pybind11 FAQ for -fvisibility flag necessity
    set(COMMON_FLAGS -I${Python_INCLUDE_DIRS} -shared -O3 -fPIC
                     -fvisibility=hidden)
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(COMMON_FLAGS ${COMMON_FLAGS} -ggdb)
    endif()

    # Gather everything into single custom target to make things a bit
    # organized in QtCreator
    add_custom_target(${P_TARGET} SOURCES ${P_SOURCES} ${P_HEADERS}
                                          ${P_TEST} ${P_PERFORMANCE} ${P_DEBUG}
                                  DEPENDS nanobind-static
    )
    add_custom_command(TARGET ${P_TARGET} PRE_BUILD
                       COMMAND ${CMAKE_CXX_COMPILER}
                       ARGS ${MOD_SOURCES} ${COMMON_FLAGS} -DENABLE_NB
                            -I${NB_DIR}/include -L. -lnanobind-static
                            -I${PROJECT_SOURCE_DIR}
                            -o ${NB_SONAME}
                       COMMENT "Compiling nanobind version of \"${P_TARGET}\""
    )
    add_custom_command(TARGET ${P_TARGET} PRE_BUILD
                       COMMAND ${CMAKE_CXX_COMPILER}
                       ARGS ${MOD_SOURCES}  ${COMMON_FLAGS} -DENABLE_PB
                            -I${pybind11_INCLUDE_DIR}
                            -I${PROJECT_SOURCE_DIR}
                            -o ${PB_SONAME}
                       COMMENT "Compiling pybind11 version of \"${P_TARGET}\""
    )

    # Add debug executables to examine extensions with gdb
    if(P_DEBUG AND (${CMAKE_BUILD_TYPE} STREQUAL "Debug"))
        #add_custom_command(TARGET ${P_TARGET} POST_BUILD
        #                   COMMAND ${CMAKE_CXX_COMPILER}
        #                           ${PROJECT_SOURCE_DIR}/${P_DEBUG}
        #                           -I${Python_INCLUDE_DIRS} -O3 -ggdb
        #                           -I${NB_DIR}/include
        #                           -fvisibility=hidden -DENABLE_NB
        #                           -I${PROJECT_SOURCE_DIR}
        #                           -l${Python_LIBRARY}
        #                           -Wl,-soname,${NB_SONAME}
        #                           -o ${P_TARGET}_nb_debug
        #                   COMMENT "Compile debug executable for nanobind ${P_TARGET}"
        #)
        add_custom_command(TARGET ${P_TARGET} POST_BUILD
                           COMMAND ${CMAKE_CXX_COMPILER}
                                   ${PROJECT_SOURCE_DIR}/${P_DEBUG}
                                   -I${Python_INCLUDE_DIRS} -O0 -ggdb
                                   -fvisibility=hidden -DENABLE_PB
                                   -I${PROJECT_SOURCE_DIR}
                                   -l${Python_LIBRARY}
                                   -Wl,-soname,${PB_SONAME}
                                   -o ${P_TARGET}_pb_debug
                           COMMENT "Compile debug executable for pybind11 ${P_TARGET}"
        )
    endif()

    # Add extra target for performance evaluation
    add_custom_target(${P_TARGET}_performance
                      SOURCES ${P_PERFORMANCE}
                      COMMAND ${Python_EXECUTABLE}
                              ${PROJECT_SOURCE_DIR}/${P_PERFORMANCE}
                      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )

    # Add tests
    add_test(NAME ${P_TARGET}
             COMMAND ${Python_EXECUTABLE} -m pytest
                     ${PROJECT_SOURCE_DIR}/${P_TEST}
             WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
endfunction()

# add common files to IDE targets
add_custom_target(common SOURCES headers/common.hxx py/basic_performance.py)

# add README.md
add_custom_target(readme SOURCES README.md)

# include project directory to make headers visible for IDE competitor
include_directories(${PROJECT_SOURCE_DIR})

include(cmake/array.cmake)
include(cmake/conversion.cmake)
include(cmake/list.cmake)
include(cmake/tree.cmake)
include(cmake/binary_tree.cmake)
include(btree/btree.cmake)
