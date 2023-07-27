#pragma once
#include "common.hxx"
#include <functional>


template<class T>
struct Element
{
    Element(const T & val, Element * nxt) :
        value(val), next(nxt) {}

    T value;
    Element * next;
};


template<class T>
class AbstractList
{
public:
    AbstractList() : _size(0), _first(nullptr), _last(nullptr) {}
    ~AbstractList() {
        if (_first == nullptr)
            return;

        std::function<Element<T> *(Element<T> *)> recursive =
            [&](Element<T> *elem) -> Element<T> *
        {
            if (elem->next != nullptr)
                delete recursive(elem->next);
            return elem;
        };
        delete recursive(_first);
    }

    Element<T> * operator [] (int idx)
    {
        if (idx < 0)
            idx += _size;

        if (! _size)
            throw py::index_error("empty list");
        else if (idx >= _size)
            throw py::index_error("list index is out of range");
        else if (! idx )
            return _first;
        else if (idx == (_size - 1))
            return _last;

        std::function<Element<T> *(Element<T> *)> recursive =
            [&](Element<T> * itr) -> Element<T> *
        {
            return (! --idx) ? itr->next : recursive(itr->next);
        };
        return recursive(_first);
    }

    inline unsigned int size () const {
        return _size;
    }

    void append (const T & value) {
        if (_first == nullptr) {
            _first = _last = new Element<T>(value, nullptr);
        } else {
            _last->next = new Element<T>(value, nullptr);
            _last = _last->next;
        }
        ++_size;
    }

    void insert (int idx, const T & value) {
        if (idx < 0)
            idx += _size;

        if (_size == 0) {
            _first = _last = new Element<T>(value, nullptr);
        } else if (idx <= 0) {
            _first = new Element<T>(value, _first);
        } else if (idx >= _size - 1) {
            _last->next = new Element<T>(value, nullptr);
            _last = _last->next;
        } else {
            Element<T> * prev = (*this)[idx-1];
            prev->next = new Element<T>(value, prev->next);
        }
        ++_size;
    }

    T pop (int idx = -1)
    {
        idx = idx < 0 ? _size - 1 : idx;
        T value;
        if (idx >= _size || ! _size) {
            throw py::index_error("pop index out of range");
        } else if (_size == 1) {
            value = _first->value;
            delete _first;
            _first = _last = nullptr;
        } else if (idx == _size-1) {
            value = _last->value;
            delete _last;
            _last = (*this)[_size-2];
            _last->next = nullptr;
        } else if (idx == 0) {
            value = _first->value;
            Element<T> * oldFirst = _first;
            _first = _first->next;
            delete oldFirst;
        } else {
            Element<T> * elemBefore = (*this)[idx-1];
            Element<T> * elemAfter = elemBefore->next->next;
            value = elemBefore->next->value;
            delete elemBefore->next;
            elemBefore->next = elemAfter;
        }
        --_size;
        return value;
    }

    /*inline T __getitem__ (const unsigned int & idx) {
        return (*this)[idx]->value;
    }

    inline T __setitem__ (AbstractList<T> & self,
                               const unsigned int & idx,
                               const T & value) {
        self[idx]->value = value;
    }*/

protected:
    size_t _size;
    Element<T> * _first;
    Element<T> * _last;
};
