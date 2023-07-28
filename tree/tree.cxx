#include "abstract_tree.hxx"
#include <functional>

struct PyNode : public Node<py::object> {
    /* This will work only with relatively simple objects.
     * Placing [1] and copy([1]) into tree will give different nodes
     * with same content */
    static inline size_t hash (const py::object & value) {
        return (size_t)value.ptr();
    }
};

struct PyTree : public PyNode, public Tree<py::object> {};

DEFINE_MODULE(MODULE_NAME(tree), m)
{
    m.def("hash", &PyNode::hash, py::arg("object"));

    py::class_<PyNode>(m, "Node")
        .def("insert", &PyNode::insert)
        .def("remove", &PyNode::remove)
        .def("__getitem__", &PyNode::operator [])
        ;

    py::class_<PyTree>(m, "Tree")
        .def(py::init<py::object, size_t>(), py::arg("object"),
             py::arg("children")=0)
        ;
}
