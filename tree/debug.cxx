#include "tree/tree.hxx"
#include <pybind11/embed.h>
#include <string>


namespace py = pybind11;

struct PyNode;
struct PyTree;

void importTree () {
    py::module_ sys = py::module_::import("sys");
    py::list PATH = sys.attr("path"); //.attr("path").cast<py::list>();
    py::str curdir = py::module_::import("os").attr("curdir");
    PATH.append(curdir);
}


int main () {
    pybind11::scoped_interpreter guard{};
    importTree();
    int root = 0;
    int rootChild0 = 1;
    int rootChild1 = 2;
    int rootChild0Child = 10;
    int rootChild1Child = 11;

    PyTree tree (py::int_(py::cast(root)));
    for (auto i = 0; i < 1000; ++i) {
        tree.insert(py::int_(i));
    }

    /*tree.insert(py::int_(15));
    tree.insert(py::int_(rootChild1));
    tree[py::int_(rootChild0)].insert(py::int_(rootChild0Child));
    tree[py::int_(rootChild1)].insert(py::int_(rootChild1Child));*/
    //tree.insert(py::int_());

    return 0;
}
