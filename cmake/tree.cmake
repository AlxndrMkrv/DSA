add_module(TARGET tree
           TEST tree/test.py
           PERFORMANCE tree/performance.py
           SOURCES tree/tree.cxx
           HEADERS headers/common.hxx
                   tree/abstract_tree.hxx
)
