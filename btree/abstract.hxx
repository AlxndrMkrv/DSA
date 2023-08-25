#pragma once
#include "headers/common.hxx"
#include <utility>


template<class T, class Derived, size_t order>
class Node {
    static constexpr size_t MinimalFruits = order/2;

public:
    //inline size_t fruitsNumber () const {
    //    return _isLeaf ? ... : _nBranches - 1; }

    void add(const T & theFruit) {

    }

private:
    /* push apart fruits array to insert the new one at the index.
     * If the node is full, the rightmost fruit will be copied with given
     * pointer
     * Note: if index >= order than add the fruit to the end. */
    void hangUpFruit(const T & theFruit, size_t index = order,
                     T * const fallenFruit = nullptr) {
        if (index < _nFruits) {
            // push apart and insert
            if (_nFruits < order) {
                std::move_backward(&_fruit[index], &_fruit[_nFruits-1],
                                   &_fruit[_nFruits]);
                ++_nFruits;
            } else {
                if (fallenFruit != nullptr)
                    *fallenFruit = _fruit[_nFruits-1];
                else
                    throw; // do something
                std::move_backward(&_fruit[index], &_fruit[_nFruits-2],
                                   &_fruit[_nFruits-1]);
            }
            _fruit[index] = theFruit;
        } else {
            // append
            if (_nFruits < order)
                _fruit[_nFruits++] = theFruit;
            else {
                if (fallenFruit != nullptr)
                    *fallenFruit = theFruit;
                else
                    throw; // do something
            }
        }
    }

private:
    bool _isLeaf;
    size_t _nBranches;
    size_t _nFruits;
    T _fruit [order];
    Node<T, Derived, order> * _branch [order];
};


template<class T, class Derived, size_t order>
class RootNode : public Node {
    static constexpr size_t MinimalFruits = 2;

public:
    void add(const T &theFruit) {

    }
};
