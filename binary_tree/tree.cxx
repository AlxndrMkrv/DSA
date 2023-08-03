#include "binary_tree/tree.hxx"


DEFINE_MODULE(MODULE_NAME(binary_tree), m)
{
    py::class_<Tree>(m, "Tree")
        .def(py::init<>())
        .def(py::init<py::object>(), py::arg("iterable"))
        .def("__len__", &Tree::__len__)
        .def("__repr__", &Tree::__repr__)
        #ifdef ENABLE_NB
        .def("__contains__", [](Tree &self, const py::object & val){
                             return self.contains(val); })
        .def("add", [](Tree &self, const py::object & val){
                    self.add(val); })
        .def("remove", [](Tree &self, const py::object & val){
                       self.remove(val); })
        .def("pop", [](Tree &self){ return self.pop(); })
        .def("clear", [](Tree &self){ self.clear(); })
        #else
        .def("__contains__", &Tree::contains)
        .def("add", &Tree::add)
        .def("remove", &Tree::remove)
        .def("pop", &Tree::pop)
        .def("clear", &Tree::clear)
        #endif
        ;
}
