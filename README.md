ABOUT
-----

This is a study project intended to:<br>
    1) improve my knowledge about Data Structures and Algorithms;<br>
    2) compare pybind11 and nanobind libraries;<br>
    3) place some demonstration of my coding style in open space.


The project separated by topics, each structure in its own directory. When the topic is done it has: <br>
    1) C++ source code to implement structure/algorithm and export the functionality into Python module;<br>
    2) test.py file based on pytest library executed by ctest;<br>
    3) performance.py file which shows some metrics based on timeit library.

Usage
-----

To see how implemented topic works just do the following

``` bash
  # create Makefiles
  $ cmake <project_dir>
  
  # build extensions
  $ make
  
  # test functionality
  $ ctest
  
  # plot performance charts (binary tree for example)
  $ make binary_tree_performance
```

Keep in mind that the performance is measuring with relatively small repeat count to reduce execution time so plots are noisy. Also all performance plots has a chart with some native python type which in most cases just gives the scale assessment and not intended to make comparison of irrelevant structures.

Progress
--------
 
 * **BTree**: still working on...
 
 * **Binary tree**: implemented. TODO: compare with stl set.
 
 * **Tree**: implemented. TODO: write tests, bugfix performance.py
 
 * **List**: implemented.
 
 * **Array**: implemented. TODO: write tests

Performance
-----------

| Structure   | Method         | nanobind  | pybind11  |
| ----------- | -------------- |:---------:|:---------:|
| array       | init()         |           | &#x1f31f; |
|             | get()          | &#x1f31f; |           |
|             | len()          | &#x1f31f; |           |
| list        | init()         |           | &#x1f31f; |
|             | append()       | &#x1f31f; |           |
|             | len()          | &#x1f31f; |           |
|             | get(middle)    | &#x1f31f; |           |
|             | insert(middle) | &#x1f31f; |           |
| tree        | init()         |           |           |
|             | insert()       |           |           |
|             | remove()       |           |           |
|             | get()          |           |           |
| binary tree | init()         |           | &#x1f31f; |
|             | contains()     | &#x1f31f; |           |

