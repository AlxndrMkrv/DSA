#pragma once
#include "common.hxx"
#include <functional>
#include <cstring>

#include <pybind11/pybind11.h>
#include <nanobind/nanobind.h>

template<class T>
struct Node
{
    // methods to implement
    virtual static size_t hash(const T & value) = 0;

    //
    Node(const T & value, Node * parent, const unsigned int & children = 0) :
        value(value), parent(parent), _children(children),
        _offspringLimit(children), _offspring(nullptr)
    {
        // acquire space for given number of children
        if (children) {
            _offspring = new Node<T> * [children];
        }
    }

    ~Node() {
        delete[] _offspring;
    }

    Node<T> & operator[] (int idx = 0)
    {
        if (idx < 0)
            idx += _children;

        if (_children == 0 || idx < 0 || idx > _children)
            throw py::index_error("index is out of range");
        else
            return *_offspring[idx];
    }

    /* QtCreator's competitor can't chew "auto & at" */
    Node<T> &(at)(int) = operator[];

    Node<T> & getChild (const T & value)
    {
        const size_t valueHash = hash(value);

    }

    void insert (const T & value)
    {
        auto idx = search_index(value);
        if (hash(value) == hash(*_offspring[idx]))
            throw py::index_error("value already presented");




    }

    void add(const T & value) {
        allocate_memory_if_necessary();
        this->at(_children++) = Node<T>(value, this);
    }



    // pop() if -_children <= idx < _children, throw exception otherwise
    /*Node<T> pop(int idx = -1) {
        if (idx < 0)
            idx += _children;

        if (! _children || idx < 0 || idx >= _children)
            throw py::index_error("pop index out of range");
        else {

        }
    }*/

protected:
    void allocate_memory_if_necessary()
    {
        // Acquire space for 2 child nodes if the node was a leaf
        if (! _offspringLimit) {
            _offspringLimit = 2;
            _offspring = new Node<T> * [_offspringLimit];
        }
        // ... and double the number if no free space available
        else if (_children == _offspringLimit) {
            const Node<T> * oldAllocation = _offspring;
            const size_t oldAllocatedNodesNumber = _offspringLimit;
            _offspringLimit *= 2;
            _offspring = new Node<T> * [_offspringLimit];
            std::memmove(_offspring, oldAllocation,
                         oldAllocatedNodesNumber * sizeof(Node<T> *));
            delete[] oldMemory;
        }
    }

    // copy all elements backward
    void push_apart(const size_t & idx)
    {
        for (auto i = _children; i > idx; --i)
            _offspring[i] = _offspring[i-1];
        _offspring[idx] = nullptr;
    }

    // find appropriate position for value in array of children
    size_t search_index(const T & value) const {
        if (_children == 0)
            return 0;

        const size_t valueHash = hash(value);
        size_t start, end = 0, _children - 1;

        if (valueHash > hash(_offspring[end]))
            return _children;
        else if (valueHash < hash(_offspring[start]))
            return 0;

        auto center = [&start, &end]() -> size_t {return (start+end)/2;};
        size_t idx;

        // Binary search complexity is O(log(n, 2)) so result will be found in
        // less than _children steps. Just avoiding while(true) loop
        while (end - start > 1) {
            idx = center();
            if (valueHash > hash(_offspring[idx]->value))
                start = idx;
            else if (valueHash < hash(_offspring[idx]->value))
                end = idx;
            else
                return idx;
        }
        return valueHash == hash(_offspring[start]->value) ? start : end;
    }

    T value;
    Node * parent;
private:
    size_t _children;
    /* Storing a children in array will lead to necessity of grandchildren
     * orphanage every time the array is sorted or resized.
     * Array of pointers solves the problem */
    Node<T> ** _offspring;
    size_t _offspringLimit;
};


template<class T>
class Tree
{
public:
    Tree(const T & rootValue = T()) : _root(Node<T>(rootValue, nullptr) {}

    Tree<T> & operator[] (int idx = 0) {
        return
    }

private:
    Node<T> _root;
}
