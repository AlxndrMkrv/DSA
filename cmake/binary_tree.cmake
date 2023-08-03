add_module(TARGET binary_tree
           TEST binary_tree/test.py
           PERFORMANCE binary_tree/performance.py
           DEBUG binary_tree/debug.cxx
           SOURCES binary_tree/tree.cxx
           HEADERS headers/common.hxx
                   binary_tree/tree.hxx
                   binary_tree/abstract_tree.hxx
)
