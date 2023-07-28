#pragma once
#include "headers/common.hxx"
#include <functional>
#include <algorithm>
#include <cstring>


template<class T>
struct Node {
    // methods to implement
    virtual static size_t hash(const T & value) = 0;

    //
    Node(const T & value, Node * parent, const unsigned int & children = 0) :
        value(value), parent(parent), _children(children),
        _offspringLimit(children), _offspring(nullptr) {
        // acquire space for given number of children
        if (children) {
            _offspring = new Node<T> * [children];
        }
    }

    ~Node() {
        for (auto i = 0; i < _children; ++i)
            delete _offspring[i];
        delete[] _offspring;
    }

    Node<T> & operator[] (const T & value)
    {
        auto idx = search_index(value);
        if (idx >= _children || _offspring[idx]->value != value)
            throw py::index_error("value not found");

        return *_offspring[idx];
    }

    Node<T> & getChild (const T & value)
    {
        const size_t valueHash = hash(value);

    }

    void insert (const T & value) {
        auto idx = search_index(value);
        if (idx < _offspring && hash(value) == hash(*_offspring[idx]))
            throw py::index_error("value already presented");

        allocate_memory_if_necessary();
        if (idx < _offspring)
            push_apart(idx);
        idx = new Node<T>(value, this);
    }

    void remove (const T & value) {

    }


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
            std::move(oldAllocation, oldAllocation + oldAllocatedNodesNumber,
                      _offspring);
            delete[] oldMemory;
        }
    }

    // copy all elements backward
    void push_apart(const size_t & idx)
    {
        std::move_backward(_offspring + idx, _offspring + _children,
                           _offspring + _children + 1);
        _offspring[idx] = nullptr;
    }

    // find appropriate position for value in array of children
    size_t search_index(const T & value) const {
        if (_children == 0)
            return 0;

        const size_t valueHash = hash(value);
        size_t start, end = 0, _children - 1;

        if (valueHash > hash(_offspring[end]->value))
            return _children;
        else if (valueHash < hash(_offspring[start]->value))
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
class Tree : public Node<T> {
    Tree(const T & value, const size_t &children = 0) :
        Node<T>(value, nullptr, children) {}
}

