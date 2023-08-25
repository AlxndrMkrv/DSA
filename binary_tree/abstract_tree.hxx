#pragma once
#include <functional>
#include <algorithm>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
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
    static inline std::string toString(const T & value) {
        return Derived::toString(value);
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
        _weight(0), _parent(nullptr), _left(nullptr), _right(nullptr) {}

public:
    // Constructor for a node. Node can't be empty.
    Node(const T & theFruit, Node<T, Derived> * parent) :
        _fruit(theFruit), _weight(1), _parent(parent),
        _left(nullptr), _right(nullptr) {}

    /* Disable copy constructor/assignment to prevent segfaults due to
     * deletion of copied Nodes */
    Node(Node &) = delete;
    Node<T, Derived> & operator=(const Node<T, Derived> &) = delete;

    /* Move constructor */
    Node(Node && other) :
        _fruit(std::move(other._fruit)),
        _weight(std::move(other._weight)),
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
            increaseWeight();
        }
    }

    // Merge togather left and right branches and return the node value
    /* Note: the other way to implement mergeBranches() is to move up the
     * leftes node in the right branch with findLeast() and abandon() methods.
     * This will let to rid of weight calculations */
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
        decreaseWeight();
        // return old value
        return tmp;
    }

    /* Remove the leftest Node and return its fruit */
    T pop() {
        if (_left) {
            if (! _left->isLeaf())
                /* delegate the problem to the left branch if it's not a leaf */
                return _left->pop();
            else {
                /* remove the leaf and delete it */
                const T tmp = _left->fruit();
                delete _left;
                _left = nullptr;
                decreaseWeight();
                return tmp;
            }
        } else if (_right) {
            /* move right branch to this if exist */
            const T tmp = _fruit;
            Node<T, Derived> * right = _right;
            _fruit = _right->fruit();
            moveBranches(_right);
            delete right;
            decreaseWeight();
            return tmp;
        } else {
            /* root without branches */
            decreaseWeight();
            return _fruit;
        }
    }


public:
    inline T fruit () const { return _fruit; }

    inline size_t fruits() const { return _weight; }

    inline bool isEmpty() const { return ! _weight; }

    inline bool isLeaf() const { return ! _left && ! _right; }

    // recursively find the node with the given fruit
    Node<T, Derived> * findNode(const T & theFruit) {
        if (isEqual(this->_fruit, theFruit))
            return this;

        Node<T, Derived> ** branch = descendant(theFruit);
        return *branch == nullptr ? nullptr
                                  : (*branch)->findNode(theFruit);
    }

    // recursively find the least fruit
    Node<T, Derived> * findLeast() {
        if (! _left)
            return this;
        else
            return _left->findLeast();
    }

    // abandon given node
    void abandon (Node<T, Derived> * node) {
        if (_left == node) {
            _left = nullptr;
            decreaseWeight();
        } else if (_right == node) {
            _right = nullptr;
            decreaseWeight();
        } else
            throw py::key_error("abandoning node is not in branches");
    }

    // rebalance. See AVL-trees
    void rebalance () {
        if (isLeaf() || (std::max(_left->fruits(), _right->fruits()) -
                         std::min(_left->fruits(), _right->fruits()) <= 1))
            return;
        // some tricky algirithm
    }

protected:
    inline void increaseWeight() {
        ++_weight;
        if (_parent) // if _parent is nullptr than 'this' is RootNode
            _parent->increaseWeight();
    }

    inline void decreaseWeight() {
        --_weight;
        if (_parent) // if _parent is nullptr than 'this' is RootNode
            _parent->decreaseWeight();
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
    size_t _weight;
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
        return "{" + Node<T, Derived>::operator std::string() + "}";
    }

    /* Clear tree */
    void clear() {
        delete this->_left;
        this->_left = nullptr;
        delete this->_right;
        this->_right = nullptr;
        this->_weight = 0;
    }

    /* Add the given fruit to tree */
    void add(const T & theFruit) {
        if (this->isEmpty()) {
            this->_fruit = theFruit;
            Node<T, Derived>::increaseWeight();
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
        if (! this->_weight)
            throw py::key_error("binary tree is empty");
        return Node<T, Derived>::pop();
    }

    /* Check if given fruit is on the tree */
    inline bool contains(const T & theFruit) {
        return this->findNode(theFruit) != nullptr;
    }
};


template<class T, class Derived>
class Serialize {
    static void exportTree (const RootNode<T, Derived> & tree,
                            const std::string & filename) {

    }
}

/*
template<class T, class Derived>
void ExportTree(const RootNode<T, Derived> & tree,
                const std::string & fileName) {
    // binary trees storing like this:
    // 0 | r | rl | rr | rll | rlr | rrl | rrr... and so on
    size_t depthReached = 0;
    size_t memoryAllocated = ;
    // allocate memory for perfectly balanced tree
    T * memory = new T [-(-tree.fruits() / 2)];

    if (pow(2, depthReached) > memoryAllocated ) {
        T * newMemory = new T [pow(2, depthReached)];

    }
    T * memory = new T [tree.fruits()];
}

template<class T, class Derived>
RootNode<T, Derived> ImportTree(const std::string & fileName) {

}

size_t ceilToNearestPowerOfTwo (const size_t & value) {
    int power = 1;
    while (power < value)
        power <<= 1;
    return power;
}
*/
