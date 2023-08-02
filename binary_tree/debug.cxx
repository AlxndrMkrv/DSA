/* A junk file to debug algorithm with gdb */

#include "binary_tree/tree.hxx"
#ifdef ENABLE_PB
#include <pybind11/embed.h>
#endif
#include <random>
#include <string>
#include <vector>

// make tree global to access it everywhere
Tree tree;

std::vector<int> sequence () {
    //return {5, 2, 10, 1, 10, 5, 8, 4, 7, 9, 7, 3, 9, 15, 11, 20};
    return {1,2,3};
}

py::object importTree () {
    py::module_ sys = py::module_::import("sys");
    py::list PATH = sys.attr("path");
    py::str curdir = py::module_::import("os").attr("curdir");
    PATH.append(curdir);
    py::module_ treeModule = py::module_::import("binary_tree_pb");
    return treeModule.attr("Tree");
}

struct IntTree : public RootNode<int, IntTree> {
    // equality static check
    static inline bool isEqual (const int &lhs, const int &rhs) {
        return lhs == rhs;
    }

    // comparison static check
    static bool isLess (const int &lhs, const int &rhs) {
        return lhs < rhs;
    }
} itree;

void clear_cxx() {
    for (int x : sequence())
        itree.add(x);
    itree.clear();
}

void remove_cxx() {
    for (int x : sequence())
        itree.add(x);

    itree.remove(1);
}

void remove_py() {
    py::object tree = importTree()();
    for (int x : sequence())
        tree.attr("add")(py::int_(x));

    std::cout << obj2str(tree.attr("__len__")()) << std::endl;
    tree.attr("remove")(py::int_(1));
    std::cout << obj2str(tree.attr("__len__")()) << std::endl;
}

void repr_cxx() {
    for (int x : sequence())
        itree.add(x);

    std::cout << static_cast<std::string>(itree) << std::endl;
}

int main () {
    py::scoped_interpreter guard{};

    clear_cxx();
    remove_cxx();
    remove_py();
    repr_cxx();

    return 0;
}
