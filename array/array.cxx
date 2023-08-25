#include "headers/common.hxx"
#include <array>
#include <functional>

constexpr unsigned int ArraySize = 10000;


class StlArray : public std::array<py::object, ArraySize>
{
public:
    StlArray () : std::array<py::object, ArraySize>() {}
    StlArray (const py::object & iterable)
    {
        unsigned int idx = 0;
        for (auto itr = iterable.begin(); itr != iterable.end(); ++itr, ++idx)
            (*this)[idx] = itr2obj(itr);
        _usedSize = idx;
    }

    inline py::object & __getitem__ (const unsigned int & idx) {
        return (*this)[idx];
    }

    inline std::string __repr__() const {
        std::stringstream sstr;
        sstr << "[";
        for (auto i = 0; i < _usedSize; ++i) {
            sstr << obj2str(this->at(i));
            if (i + 1 < _usedSize)
                sstr << ", ";
        }
        sstr << "]";
        return sstr.str();
    }

private:
    unsigned int _usedSize;
};


DEFINE_MODULE(MODULE_NAME(array), m)
{
    py::class_<StlArray>(m, "StlArray")
        .def(py::init<>())
        .def(py::init<py::object>(), py::arg("iterable"))
        .def("__repr__", &StlArray::__repr__)
        .def("__len__", [](StlArray &self){return self.size();})
        .def("__getitem__", &StlArray::__getitem__)
        ;
}

