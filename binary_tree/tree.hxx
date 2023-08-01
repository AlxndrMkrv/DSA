#pragma once
#include "abstract_tree.hxx"

#include <pybind11/pybind11.h>

struct PyNode : public Node<py::object, PyNode> {
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


struct PyTree : public RootNode<py::object, PyTree> {
    PyTree(const T &)
};
