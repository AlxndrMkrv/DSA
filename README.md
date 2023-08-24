ABOUT
=====

This is a study project intended to:
    1) improve my knowledge about Data Structures and Algorithms;
    2) compare pybind11 and nanobind libraries;
    3) place some demonstration of my coding style in open space.

The project separated by topics: each structure in its own directory. When the topic is done it has:
    1) C++ source code to implement structure/algorithm and export the functionality into Python module;
    2) test.py file based on pytest library binded to ctest;
    3) performance.py file which shows some metrics based on timeit library.

Usage
=====

To see how implemented topic works just do the following

    $ cmake <project_dir>
    $ make
    $ ctest
    $ make binary_tree_performance **for example**

Keep in mind that the performance measuring with relative small repeat count to reduce execution time so plots are noisy. Also all performance plots has chart with some native python type which in most cases just gives the scale assessment and not intended to make comparison of irrelevant structures.

Progress
========

*Binary tree* completely implemented. Interesting fact: nanobind gives lower performance on structure initialization than pybind11.


