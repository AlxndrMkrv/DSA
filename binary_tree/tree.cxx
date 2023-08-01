//#include "binary_tree/tree.hxx"
#include "binary_tree/abstract_tree.hxx"


struct Tree : public RootNode<py::object, Tree> {
    Tree() : RootNode<py::object, Tree>() {}

    /*static bool isEqual (const py::object &lhs, const py::object &rhs) {
        return *const_cast<py::object*>(&lhs) ==
               *const_cast<py::object*>(&rhs);
    }

    static bool isLess (const py::object &lhs, const py::object &rhs) {
        return lhs < rhs;
    }*/
    /*bool operator < (const py::object & value) const {
        return this->value() < value;
    }

    bool operator == (const py::object & value) const {
        return this->value() == value;
    }*/

    static constexpr std::string (&toString)(const py::object &) = obj2str;

    size_t __len__() const {
        return _nFruits;
    }

    std::string __repr__() {
        return static_cast<std::string>(*this);
    }
};


DEFINE_MODULE(MODULE_NAME(binary_tree), m)
{
    py::class_<Tree>(m, "Tree")
        .def(py::init<>())
        .def("__len__", &Tree::__len__)
        .def("__repr__", &Tree::__repr__)
        .def("__contains__", &Tree::contains)
        #ifdef ENABLE_NB
        .def("add", [](Tree &self, const py::object & val){
                    return self.add(val);})
        #else
        .def("add", &Tree::add)
        #endif
        /*.def("remove", &Tree::remove)
        .def("pop", &Tree::pop)*/
        ;
}
