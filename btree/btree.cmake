add_module(TARGET btree
           TEST btree/test.py
           PERFORMANCE btree/performance.py
           DEBUG btree/debug.cxx
           SOURCES btree/extension.cxx
           HEADERS headers/common.hxx
                   btree/extension.hxx
                   btree/abstract.hxx
)
