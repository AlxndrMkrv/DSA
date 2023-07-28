#include "abstract_list.hxx"
#include <list>

class List : public AbstractList<py::object>
{
public:
    List() : AbstractList<py::object>() {}
    List(const py::object & iterable) : AbstractList<py::object>()
    {
        for (auto itr = iterable.begin(); itr != iterable.end(); ++itr)
            append(itr2obj(itr));
    }

    std::string __repr__ () {
        std::stringstream sstr;
        sstr << "[";
        for (auto ptr = _first; ptr != nullptr; ptr = ptr->next)
            sstr << obj2str(ptr->value)
                 << (ptr->next == nullptr ? "" : ", ");
        sstr << "]";
        return sstr.str();
    }
};


struct StlList : public std::list<py::object>
{
    StlList() : std::list<py::object>() {}
    StlList(const py::object & iterable) : std::list<py::object>()
    {
        for (auto itr = iterable.begin(); itr != iterable.end(); ++itr)
            append(itr2obj(itr));
    }

    std::string __repr__ () {
        std::stringstream sstr;
        sstr << "[";
        auto itr = this->begin();
        while (itr != this->end()) {
            sstr << obj2str(*itr);
            if (std::next(itr, 1) != this->end())
                sstr << ", ";
            ++itr;
        }
        sstr << "]";
        return sstr.str();
    }

    inline void append(const py::object & value) {
        this->push_back(value);
    }

    inline void insert(int & idx, const py::object & value) {
        std::list<py::object>::insert(std::next(this->begin(), idx), value);
    }

    inline py::object pop(int idx = -1) {
        idx = idx < 0 ? std::size(*this) - 1 : idx;
        if (idx >= std::size(*this) || ! std::size(*this))
            throw py::index_error("pop index out of range");

        auto itr = std::next(this->begin(), idx);
        const py::object value = *itr;
        std::list<py::object>::erase(itr);
        return value;
    }

    inline py::object __getitem__ (const unsigned int & idx) const {
        return *std::next(this->begin(), idx);
    }

    inline void __setitem__ (const unsigned int & idx,
                             const py::object & value) {
        *std::next(this->begin(), idx) = value;
    }

    inline int __size__ () const {
        return this->size();
    }
};


DEFINE_MODULE(MODULE_NAME(list), m)
{
    py::class_<List>(m, "MyList")
        .def(py::init<>())
        .def(py::init<py::object>(), py::arg("iterable"))
        .def("__repr__", &List::__repr__)
        #ifdef ENABLE_NB
        .def("append", [](List & self, py::object & obj){self.append(obj);})
        .def("__len__", [](const List & self){return self.size();})
        .def("insert", [](List & self, const int & idx,
                          py::object & obj){self.insert(idx, obj);})
        .def("pop", [](List &self, int & idx){return self.pop(idx);},
            py::arg("idx") = -1)
        #else
        .def("append", &List::append)
        .def("__len__", &List::size)
        .def("insert", &List::insert)
        .def("pop", &List::pop, py::arg("idx") = -1)
        #endif
        .def("__getitem__", [](List & self, int idx){return self[idx]->value;})
        .def("__setitem__", [](List & self, int idx,
                               const py::object & obj) {
                                   self[idx]->value = obj;},
             "Set list element",
             py::arg("idx"),
             py::arg("object")
        )
        ;

    py::class_<StlList>(m, "StlList")
        .def(py::init<>())
        .def(py::init<py::object>(), py::arg("iterable"))
        .def("__repr__", &StlList::__repr__)
        .def("append", &StlList::append)
        .def("__len__", &StlList::__size__)
        .def("__getitem__", &StlList::__getitem__)
        .def("__setitem__", &StlList::__setitem__)
        .def("insert", &StlList::insert)
        .def("pop", &StlList::pop, py::arg("idx") = -1)
        ;
}

