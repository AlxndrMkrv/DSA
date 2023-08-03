#pragma once
#include "headers/common.hxx"


template<class T, class Derived, size_t order>
class Node {
    static constexpr size_t MinimalFruits = order/2;

public:
    //inline size_t fruitsNumber () const {
    //    return _isLeaf ? ... : _nBranches - 1; }

private:
    bool _isLeaf;
    size_t _nBranches;
    T _fruit [order];
    Node<T, Derived, order> * _branch [order];
};


template<class T, class Derived, size_t order>
class RootNode : public Node {
    static constexpr size_t MinimalFruits = 2;
};
