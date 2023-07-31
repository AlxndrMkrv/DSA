#pragma once
#include "abstract_tree.hxx"


struct PyNode : public Node<py::object, PyNode> {
    /* This will work only with relatively simple objects.
     * Placing [1] and copy([1]) into tree will give different nodes
     * with same content
     */
    /* Note that using id() is not a good idea:
     * a, b = 257, 257
     * assert id(a) == id(b)
     * a = 257
     * b = 257
     * assert id(a) == id(b)
     */
    static constexpr size_t (&hash)(const py::object &) = id;



    static constexpr std::string (&toString)(const py::object &) = obj2str;

    PyNode(const py::object & value, Node * parent,
           const unsigned int & children = 0) :
        Node<py::object, PyNode>(value, parent, children) {}

    PyNode & __getitem__(const py::object & value) {
        return static_cast<PyNode &>((*this)[value]);
    }

    size_t __len__() const {
        return _children;
    }
};


struct PyTree : public PyNode {
    PyTree(const py::object & value, const size_t & children = 0) :
        PyNode(value, nullptr, children) {}
};
