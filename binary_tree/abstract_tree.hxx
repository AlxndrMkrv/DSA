#pragma once
#include <functional>
#include <algorithm>
#include <cstring>
#include <string>
#include <iostream>
#include "headers/common.hxx"


/* This tree implementation is relied on '<' and '==' operations which must be
 * implemented in Derived class. Thus it can hold only comparable types.
 *
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
    inline std::string toString(const T & value) const {
        return static_cast<const Derived *>(this)->toString(value);
    }

    static inline bool isEqual (const T & lhs, const T & rhs) {
        return Derived::isEqual(lhs, rhs);
    }

    static inline bool isLess (const T & lhs, const T & rhs) {
        return Derived::isLess(lhs, rhs);
    }

    /* ======================= Node implementation ====================== */
protected:
    // Constructor for a fruitless root node. Root may be empty.
    Node() :
        _nFruits(0), _parent(nullptr), _left(nullptr), _right(nullptr) {}

public:
    // Constructor for a node. Node can't be empty.
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
        moveBranches(&other);
        return *this;
    }

    ~Node() {
        delete _left;
        delete _right;
    }

    /* Move left and right branches */
    void moveBranches(Node<T, Derived> * other) {
        _left = std::move(other->_left);
        _right = std::move(other->_right);
        if (other->_left) {
            other->_left->relocate(this);
            other->_left = nullptr;
        }
        if (other->_right) {
            other->_right->relocate(this);
            other->_right = nullptr;
        }
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
        // return silently if fruit already present
        if (isEqual(this->_fruit, theFruit)) {
            return;
        }

        Node<T, Derived> ** child = descendant(theFruit);
        // pass the fruit to the next node if exist or create new
        if (*child)
            (*child)->add(theFruit);
        else {
            *child = new Node<T, Derived>(theFruit, this);
            incrementFruitsNumber();
        }
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
                 _right->fruits() > _left->fruits()) ? _right : _left;
        Node<T, Derived> * weak = (strong == _left) ? _right : _left;

        // move strong branch to this
        _fruit = std::move(strong->_fruit);
        moveBranches(strong);
        // delete strong branch
        delete strong;
        // adopt weak branch if it's not a leaf
        if (weak)
            adoptOrphanBranch(weak);
        // one node goes away so decrement children
        decrementFruitsNumber();
        // return old value
        return tmp;
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


public:
    inline T fruit () const {
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

    Node<T, Derived> * findNode(const T & theFruit) {
        if (isEqual(this->_fruit, theFruit))
            return this;

        Node<T, Derived> ** branch = descendant(theFruit);
        return *branch == nullptr ? nullptr
                                  : (*branch)->findNode(theFruit);
    }

protected:
    inline void incrementFruitsNumber() {
        ++_nFruits;
        if (_parent) // if _parent is nullptr than 'this' is RootNode
            _parent->incrementFruitsNumber();
    }

    inline void decrementFruitsNumber() {
        --_nFruits;
        if (_parent) // if _parent is nullptr than 'this' is RootNode
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

    /* Clear tree */
    void clear() {
        delete this->_left;
        this->_left = nullptr;
        delete this->_right;
        this->_right = nullptr;
        this->_nFruits = 0;
    }

    /* Add the given fruit to tree */
    void add(const T & theFruit) {
        if (this->isEmpty()) {
            this->_fruit = theFruit;
            Node<T, Derived>::incrementFruitsNumber();
        } else
            Node<T, Derived>::add(theFruit);
    }

    /* Removing the given fruit from tree */
    void remove(const T & theFruit) {
        if (this->isEmpty())
            throw py::key_error("removing from empty tree");

        Node<T, Derived> * nodeToRemove = this->findNode(theFruit);
        if (! nodeToRemove)
            throw py::key_error("value not presented");

        nodeToRemove->mergeBranches();
    }

    /* Remove and return the least fruit from tree */
    T pop() {
        if (! this->_nFruits)
            throw py::key_error("binary tree is empty");
        Node<T, Derived>::pop();
    }

    /* Check if given fruit is on the tree */
    inline bool contains(const T & theFruit) {
        return this->findNode(theFruit) != nullptr;
    }
};
