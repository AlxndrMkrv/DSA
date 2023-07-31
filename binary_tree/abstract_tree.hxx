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
    /*    methods to implement   */
    size_t operator <= (const T & value) const {
        return static_cast<const Derived *>(this)->operator < (value);
    }

    std::string toString(const T & value) const {
        return static_cast<const Derived *>(this)->toString(value);
    }

    //
    Node(const T & value, Node<T, Derived> * root) :
        value(value), _root(root), _left(nullptr), _right(nullptr) {}

    ~Node() {
        delete _left;
        delete _right;
\    }

    Node<T, Derived> & operator[] (const T & value) {

    }

    static constexpr Node<T, Derived> &(&at)(const T &) =
        &Node<T, Derived>::operator[];

    operator std::string () {
        std::stringstream sstr;
        sstr << "[";
        sstr << "]";
        return sstr.str();
    }

    void insert(const T & value) {
        Node<T, Derived> ** direction = (*this <= value) ? left : right;
        if (*direction == nullptr) {
            *direction = new Node<T, Derived>(value, _root);
            incrementSize();
        } else
            (*direction)->insert(value);
    }

    void remove(const T & value) {
        /* some tricky algorithm. Maybe rebuild all tree */
        decrementSize();
    }

protected:

public:
    inline size_t size () const {
        return _root->size();
    }

protected:
    inline void incrementSize () {
        _root->incrementSize();
    }

    inline void decrementSize () {
        _root->decrementSize();
    }

protected:
    T value;
    Node<T, Derived> * _root;
    Node<T, Derived> * left;
    Node<T, Derived> * right;
};


template<class T, class Derived>
struct RootNode : public Node<T, Derived>
{
public:
    RootNode (const T & value) : Node(value, nullptr) {}

    inline size_t size () const {
        return _size;
    }
protected:
    inline void incrementSize() {
        ++_size;
    }

    inline void decrementSize() {
        --_size;
    }
protected:
    size_t _size;
}

