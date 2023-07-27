add_module(TARGET list
           TEST list/test.py
           PERFORMANCE list/performance.py
           SOURCES list/list.cxx
           HEADERS headers/common.hxx
                   list/abstract_list.hxx
)
