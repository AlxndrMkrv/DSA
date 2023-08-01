#include "tree/tree.hxx"


DEFINE_MODULE(MODULE_NAME(tree), m)
{
    m.def("hash", &id, py::arg("object"));

    py::class_<PyNode>(m, "Node")
        .def("__getitem__", &PyNode::__getitem__, py::rv_policy::reference)
        .def("__len__", &PyNode::__len__)
        #ifdef ENABLE_NB
        .def("insert", [](PyNode & self,
                          py::object & obj){return self.insert(obj);})
        .def("remove", [](PyNode & self,
                          py::object & obj){return self.remove(obj);})
        .def("__repr__", [](PyNode & self){return self.operator std::string();})
        #else
        .def("insert", &PyNode::insert)
        .def("remove", &PyNode::remove)
        .def("__repr__", &PyNode::operator std::string)
        #endif
        ;

    py::class_<PyTree, PyNode>(m, "Tree")
        .def(py::init<py::object, unsigned long long>(), py::arg("object"),
             py::arg("children")=0)
        ;
}
