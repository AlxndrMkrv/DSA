#include "common.hxx"

template<class T>
class Tree
{
// Tree will work only with relatively simple objects.
// Placing [1] and copy([1]) into tree will give different nodes with same content
};

#include <functional>


DEFINE_MODULE(MODULE_NAME(tree), m)
{
    m.def("hash", [](py::object obj){return std::hash<py::object>(obj.ptr());});
    /*
    py::class_<Tree>(m, "Tree")
        .def(py::init<>())
        .def(py::init<py::object>(), py::arg("iterable"))
        .def("__repr__", &StlArray::__repr__)
        .def("__len__", [](StlArray &self){return self.size();})
        .def("__getitem__", &StlArray::__getitem__)
        ;*/
}
