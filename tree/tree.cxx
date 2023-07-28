#include "abstract_tree.hxx"
#include <functional>


struct PyNode : public Node<py::object, PyNode> {
    /* This will work only with relatively simple objects.
     * Placing [1] and copy([1]) into tree will give different nodes
     * with same content */
    static constexpr size_t (&hash)(const py::object &) = id;
    static constexpr std::string (&to_string)(const py::object &) = obj2str;

    PyNode(const py::object & value, Node * parent,
           const unsigned int & children = 0) :
        Node<py::object, PyNode>(value, parent, children) {}
};


struct PyTree : public PyNode {
    PyTree(const py::object & value, const size_t & children = 0) :
        PyNode(value, nullptr, children) {}
};


DEFINE_MODULE(MODULE_NAME(tree), m)
{
    m.def("hash", &id, py::arg("object"));

    py::class_<PyNode>(m, "Node")
        #ifdef ENABLE_NB
        .def("insert", [](PyNode & self,
                          py::object & obj){return self.insert(obj);})
        .def("remove", [](PyNode & self,
                          py::object & obj){return self.remove(obj);})
        .def("__getitem__", [](PyNode & self,
                               py::object & obj){return self[obj];})
        .def("__repr__", [](PyNode & self){return self.operator std::string();})
        #else
        .def("insert", &PyNode::insert)
        .def("remove", &PyNode::remove)
        .def("__getitem__", &PyNode::operator [])
        .def("__repr__", &PyNode::operator std::string)
        #endif
        ;

    py::class_<PyTree, PyNode>(m, "Tree")
        .def(py::init<py::object, unsigned long long>(), py::arg("object"),
             py::arg("children")=0)
        ;
}
