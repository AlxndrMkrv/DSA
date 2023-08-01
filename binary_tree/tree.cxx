//#include "binary_tree/tree.hxx"
#include "binary_tree/abstract_tree.hxx"


struct Tree : public RootNode<T, Derived> {
    Tree();

    bool operator < (const py::object & value) const {
        return this->value() < value;
    }

    bool operator == (const py::object & value) const {
        return this->value() == value;
    }

    static constexpr std::string (&toString)(const py::object &) = obj2str;

    size_t __len__() const {
        return _nFruits;
    }
};


DEFINE_MODULE(MODULE_NAME(binary_tree), m)
{
    py::class_<Tree>(m, "Tree")
        .def(py::init<>())
        .def("__len__", &Tree::__len__)
        .def("__contains__", &Tree::contains)
        .def("add", &Tree::add)
        .def("remove", &Tree::remove)
        .def("pop", &Tree::pop)
        ;
}
