#pragma once
#include "binary_tree/abstract_tree.hxx"

struct Tree : public RootNode<py::object, Tree> {
    // Construct empty tree
    Tree() : RootNode<py::object, Tree>() {}

    // Construct tree from iterable
    Tree(const py::object & iterable) : RootNode<py::object, Tree>() {
        for (auto itr = iterable.begin(); itr != iterable.end(); ++itr)
            add(itr2obj(itr));
    }

    // equality static check
    static inline bool isEqual (const py::object &lhs, const py::object &rhs) {
        return lhs.equal(rhs);
    }

    // comparison static check
    static bool isLess (const py::object &lhs, const py::object &rhs) {
        return lhs < rhs;
    }

    // alias
    static constexpr std::string (&toString)(const py::object &) = obj2str;


    inline size_t __len__() const {
        return _nFruits;
    }

    inline std::string __repr__() {
        return static_cast<std::string>(*this);
    }
};
