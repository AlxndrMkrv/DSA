#pragma once
#include <functional>
#include <algorithm>
#include <cstring>
#include <string>
#include <iostream>
#include "headers/common.hxx"


/* This binary tree is kinda implementing python 'set'.
 * Current implementation will work fine if filled with unordered random data.
 * However feeding it with ordered data array will transform it to
 * kind of linked list with O(n) access complexity.
 *
 * TODO: implementation of rebalancing algorithm to use after data feeding
 * will help to reduct the worst complexity */

template<class T, class Derived>
class Node {
public:
    /* ========================= CRTP interface ========================= */
    std::string toString(const T & value) const {
        return static_cast<const Derived *>(this)->toString(value);
    }

    static bool isEqual (const T & lhs, const T & rhs) {
        return Derived::isEqual(lhs, rhs);
    }

    static bool isLess (const T & lhs, const T & rhs) {
        return Derived::isLess(lhs, rhs);
    }

    /* ======================= Node implementation ====================== */
protected:
    // Constructor for uninitialized root node
    Node() :
        _nFruits(0), _parent(nullptr), _left(nullptr), _right(nullptr) {}

public:
    // Common constructor
    Node(const T & theFruit, Node<T, Derived> * parent) :
        _fruit(theFruit), _nFruits(1), _parent(parent),
        _left(nullptr), _right(nullptr) {}

    /* Disable copy constructor/assignment to prevent segfaults due to
     * deletion of copied Nodes */
    Node(Node &) = delete;
    Node<T, Derived> & operator=(const Node<T, Derived> &) = delete;

    /* Move constructor */
    Node(Node && other) :
        _fruit(std::move(other._fruit)),
        _nFruits(std::move(other._nFruits)),
        _parent(std::move(other._parent)),
        _left(std::move(other._left)),
        _right(std::move(other._right)) {
        if (other._left) {
            other._left->relocate(this);
            other._left = nullptr;
        }
        if (other._right) {
            other._right->relocate(this);
            other._right = nullptr;
        }
    }

    /* Move assignment must care only about branch pointers. Other objects
     * handling depends on situation */
    Node<T, Derived> & operator=(Node<T, Derived> && other) {
        _left = std::move(other._left);
        _right = std::move(other._right);
        if (other._left) {
            other._left->relocate(this);
            other._left = nullptr;
        }
        if (other._right) {
            other._right->relocate(this);
            other._right = nullptr;
        }
        return *this;
    }

    ~Node() {
        delete _left;
        delete _right;
    }

    /* Concatenate fruits from branches with node fruit */
    operator std::string() {
        std::string buffer;
        if (_left != nullptr)
            buffer += static_cast<std::string>(*_left) + ", ";
        buffer += toString(_fruit);
        if (_right != nullptr)
            buffer += ", " + static_cast<std::string>(*_right);
        return buffer;
    }

    /* Change the node parent for rearrange purposes */
    void relocate(Node<T, Derived> * newParent) {
        _parent = newParent;
    }

    /* Add new fruit to the tree */
    void add(const T & theFruit) {
        // return silently if value already present
        if (isEqual(this->_fruit, theFruit)) {
            return;
        }

        Node<T, Derived> ** child = descendant(theFruit);
        // pass value to the next node if exist or create new
        if (*child)
            (*child)->add(theFruit);
        else {
            *child = new Node<T, Derived>(theFruit, this);
            incrementFruitsNumber();
        }
    }

    /* remove the least Node and return it's value */
    T pop() {
        if (_left->isEmpty()) {
            /* delete left node if it's a leaf */
            const T tmp = _left->_value;
            delete _left;
            decrementFruitsNumber();
            _left = nullptr;
            return tmp;
        } else if (! _left->_left) {
            /* move left's right node up if there's no left's left */
            const T tmp = _left->_value;
            Node<T, Derived> * newLeft = _left->_right;
            newLeft->relocate(this);
            delete _left;
            _left = newLeft;
            decrementFruitsNumber();
            return tmp;
        } else {
            /* recursively call pop() down on the left branch */
            _left->pop();
        }
    }

    bool contains(const T & value) {
        return findNode(value) != nullptr;
    }


public:
    inline T value () const {
        return _fruit;
    }

    inline size_t fruits() const {
        return _nFruits;
    }

    inline bool isEmpty() const {
        return ! _nFruits;
    }

    inline bool isLeaf() const {
        return ! _left && ! _right;
    }

protected:
    inline void incrementFruitsNumber() {
        ++_nFruits;
        _parent->incrementFruitsNumber();
    }

    inline void decrementFruitsNumber() {
        --_nFruits;
        _parent->decrementFruitsNumber();
    }

    void adoptOrphanBranch(Node<T, Derived> * orphan) {
        Node<T, Derived> ** child = descendant(orphan->_fruit);
        if (! *child) {
            *child = orphan;
            orphan->relocate(this);
        } else
            (*child)->adoptOrphanBranch(orphan);
    }

    Node<T, Derived> * findNode(const T & theFruit) {
        if (isEqual(this->_fruit, theFruit))
            return this;

        Node<T, Derived> ** branch = descendant(theFruit);
        return *branch == nullptr ? nullptr
                                  : (*branch)->findNode(theFruit);
    }

    // Merge togather left and right branches and return the node value
    T mergeBranches() {
        if (isLeaf())
            return _fruit;

        /* make temporary copy of current value and
         * decide which branch has more weight */
        const T tmp = _fruit;
        Node<T, Derived> * strong = ! _left ||
                (_left && _right &&
                 _right->children() > _left->children()) ? _right : _left;
        Node<T, Derived> * weak = strong == _left ? _right : _left;

        _fruit = strong->_fruit;
        // move here left and right branches of strongest
        _left = strong->_left;
        if (_left) {
            _left->relocate(this);
            strong->_left = nullptr;
        }
        _right = strong->_right;
        if (_right) {
            _right->relocate(this);
            strong->_right = nullptr;
        }
        // delete strong branch node and adopt weak branch
        delete strong;
        adoptOrphanBranch(weak);
        // one node goes away so decrement children
        decrementFruitsNumber();
        // return old value
        return tmp;
    }

private:
    inline Node<T, Derived> ** descendant (const T & theFruit) {
        return (isLess(theFruit, this->_fruit)) ? &_left : &_right;
    }

protected:
    T _fruit;
    size_t _nFruits;
    Node<T, Derived> * _parent;
    Node<T, Derived> * _left;
    Node<T, Derived> * _right;
};


template<class T, class Derived>
struct RootNode : public Node<T, Derived>
{
public:
    RootNode () : Node<T, Derived>() {}
    RootNode (const T & theFruit) : Node<T, Derived>(theFruit, nullptr) {}

    operator std::string () {
        return "{" + static_cast<std::string>(*this) + "}";
    }

    /* Add the given value to tree */
    void add(const T & theFruit) {
        if (this->isEmpty()) {
            this->_fruit = theFruit;
            incrementFruitsNumber();
        } else
            Node<T, Derived>::add(theFruit);
    }

    /* Removing the given value from tree */
    void remove(const T & theFruit) {
        if (this->isEmpty())
            throw py::key_error("removing from empty tree");

        Node<T, Derived> * nodeToRemove = this->findNode(theFruit);
        if (! nodeToRemove)
            throw py::key_error("value not presented");

        nodeToRemove->mergeBranches();
    }

    /* Remove and return the lowerest value from tree */
    T pop() {
        if (! this->_nFruits)
            throw py::key_error("binary tree is empty");
        Node<T, Derived>::pop();
    }

protected:
    inline void incrementFruitsNumber() {
        ++this->_nFruits;
    }

    inline void decrementFruitsNumber() {
        --this->_nFruits;
    }
};

