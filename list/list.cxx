#include "abstract_list.hxx"
#include <list>
#include <sstream>


class List : public AbstractList<py::object>
{
public:
    std::string __repr__ () {
        std::stringstream sstr;
        std::function<std::string(Element<py::object> *)> recursive =
            [&recursive](Element<py::object> * elem) -> std::string {
            return elem == nullptr ? ""
                                   : obj2str(elem->value) +
                                     (elem->next == nullptr ? ""
                                                            : ", " + recursive(elem->next));
        };
        sstr << "[" << recursive(_first) << "]";
        return sstr.str();
    }

    static inline void append2 (List & self, const py::object & obj) {
        self.append(obj);
    }
};


struct StlList : public std::list<int>
{
    StlList() : std::list<int>() {}
    StlList(const unsigned int & size) : std::list<int>(size, 0) {}

    std::string __repr__ () {
        std::stringstream sstr;
        sstr << "[";
        auto itr = this->begin();
        while (itr != this->end()) {
            sstr << std::to_string(*itr);
            if (std::next(itr, 1) != this->end())
                sstr << ", ";
            ++itr;
        }
        sstr << "]";
        return sstr.str();
    }

    inline void append(const int & value) {
        this->push_back(value);
    }

    inline void insert(const unsigned int & idx, const int & value) {
        std::list<int>::insert(std::next(this->begin(), idx), value);
    }

    inline int pop(int idx = -1) {
        idx = idx < 0 ? std::size(*this) - 1 : idx;
        if (idx >= std::size(*this) || ! std::size(*this))
            throw py::index_error("pop index out of range");

        auto itr = std::next(this->begin(), idx);
        const int value = *itr;
        std::list<int>::erase(itr);
        return value;
    }

    inline int __getitem__ (const unsigned int & idx) {
        return *std::next(this->begin(), idx);
    }

    inline void __setitem__ (const unsigned int & idx,
                             const int & value) {
        *std::next(this->begin(), idx) = value;
    }

    inline int __size__ () {
        return std::size(*this);
    }
};


DEFINE_MODULE(MODULE_NAME(list), m)
{
    py::class_<List>(m, "MyList")
        .def(py::init<>())
        .def(py::init<unsigned int &>())
        .def("__repr__", &List::__repr__)
        #ifdef ENABLE_NB
        .def("append", &List::append2)
        .def_static("test", [](const py::object &obj){return obj2str(obj);})
        #elif ENABLE_PB
        .def("append", &List::append)
        #endif
        .def("__len__", &List::__len__)
        .def("__getitem__", &List::__getitem__)
        .def("__setitem__", &List::__setitem__)
        .def("insert", &List::insert)
        .def("pop", &List::pop, py::arg("idx") = -1)
        ;

    py::class_<StlList>(m, "StlList")
        .def(py::init<>())
        .def(py::init<unsigned int>())
        .def("__repr__", &StlList::__repr__)
        .def("append", &StlList::append)
        .def("__len__", &StlList::__size__)
        .def("__getitem__", &StlList::__getitem__)
        .def("__setitem__", &StlList::__setitem__)
        .def("insert", &StlList::insert)
        .def("pop", &StlList::pop, py::arg("idx") = -1)
        ;
}

