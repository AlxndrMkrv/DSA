#pragma once
#include <functional>
#include <algorithm>
#include <cstring>
#include <string>
#include <iostream>
#include "headers/common.hxx"


template<class T, class Derived>
class Node {
public:
    /* ========================= CRPT interface ========================= */
    size_t hash(const T & value) const {
        return static_cast<const Derived *>(this)->hash(value);
    }

    std::string toString(const T & value) const {
        return static_cast<const Derived *>(this)->toString(value);
    }

    //
    Node(const T & value, Node * parent, const unsigned int & children = 0) :
        value(value), parent(parent), _children(0),
        _offspringLimit(children), _offspring(nullptr) {
        // acquire space for given number of children
        if (children) {
            _offspring = new Node<T, Derived> * [children];
        }
    }

    ~Node() {
        for (auto i = 0; i < _children; ++i)
            delete _offspring[i];
        delete[] _offspring;
    }

    Node<T, Derived> & operator[] (const T & value) {
        auto idx = searchIndex(value);
        if (idx >= _children || hash(_offspring[idx]->value) != hash(value))
            throw py::index_error(("value " + toString(value) +
                                   " not found").c_str());
        return *_offspring[idx];
    }

    static constexpr Node<T, Derived> &(&at)(const T &) =
            &Node<T, Derived>::operator[];

    operator std::string () {
        std::stringstream sstr;
        sstr << toString(value) << ": {";
        for (auto i = 0; i < _children; ++i) {
            sstr << toString(_offspring[i]->value)
                 << ((i + 1) < _children ? ", " : "");
        }
        sstr << "}";
        return sstr.str();
    }

    void insert(const T & value) {
        auto idx = searchIndex(value);
        if (idx < _children && hash(value) == hash(_offspring[idx]->value))
            throw py::index_error(("value '" + toString(value) +
                                   "' already present").c_str());

        allocateMemoryIfNecessary();
        if (idx < _children)
            pushApart(idx);
        _offspring[idx] = new Node<T, Derived>(value, this);
        ++_children;
    }

    void remove(const T & value) {
        // find given value in _children and remove it if exists
        auto idx = searchIndex(value);
        if (idx >= _children || hash(value) != hash(_offspring[idx]->value))
            throw py::index_error(("value '" + toString(value) +
                                   "' not present").c_str());

        delete _offspring[idx];
        pullTogather(idx);
    }


protected:
    void allocateMemoryIfNecessary() {
        if (!_offspringLimit) {
            // Acquire space for 2 child nodes if the node was a leaf
            _offspringLimit = 2;
            _offspring = new Node<T, Derived> * [_offspringLimit];
        } else if (_children == _offspringLimit) {
            // ... and double the number if no free space available
            Node<T, Derived> ** const oldAllocation = _offspring;
            const size_t oldAllocatedNodesNumber = _offspringLimit;
            _offspringLimit *= 2;
            _offspring = new Node<T, Derived> * [_offspringLimit];
            std::move(oldAllocation, oldAllocation + oldAllocatedNodesNumber,
                      _offspring);
            delete[] oldAllocation;
        }
    }

    void pushApart(const size_t & idx) {
        // shift pointers so idx will be free for insertion
        std::move_backward(_offspring + idx, _offspring + _children,
                           _offspring + _children + 1);
        _offspring[idx] = nullptr;
    }

    void pullTogather(const size_t & idx) {
        //
        std::move(_offspring + idx + 1, _offspring + _children,
                  _offspring + idx);
    }

    /* Find appropriate position for value in array of children.
     * Returns index which
     * hash(_children[index]) <= hash(value) < hash(children[index+1])
     * so returned value requires addition check for insertion and getting */
    size_t searchIndex(const T & value) const {
        if (_children == 0)
            return 0;

        const size_t valueHash = hash(value);
        size_t start = 0;
        size_t end = _children - 1;

        if (valueHash > hash(_offspring[end]->value))
            return _children;
        else if (valueHash < hash(_offspring[start]->value))
            return 0;

        auto center = [&start, &end]() -> size_t {return (start+end)/2;};

        // O(log(n, 2)) complexity
        while (end - start > 1) {
            const size_t idx = center();
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

protected:
    size_t _children;
    /* Storing a children directly in array will lead to grandchildren orphanage
     * every time the array is sorted or resized.
     * Array of pointers solves the problem */
    Node<T, Derived> ** _offspring;
    size_t _offspringLimit;
};
