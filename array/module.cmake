add_module(TARGET array
           TEST array/test.py
           PERFORMANCE array/performance.py
           SOURCES array/array.cxx
           HEADERS common.hxx
)
