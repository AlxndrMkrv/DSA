#include "../common.hxx"
#include <functional>
#include <list>
#include <sstream>


struct Element
{
    Element(const int & val, Element * nxt) :
        value(val), next(nxt) {}

    int value;
    Element * next;
};


class List
{
public:
    List() : _size(0), _first(nullptr), _last(nullptr) {}
    List(unsigned int & n) : List() {
        for (auto i = 0; i < n; ++i)
            this->append(0);
    }
    ~List() {
        if (_first == nullptr)
            return;

        std::function<Element *(Element*)> recursive =
            [&](Element *elem) -> Element *
        {
            if (elem->next != nullptr)
                delete recursive(elem->next);
            return elem;
        };
        delete recursive(_first);
    }

    std::string __repr__ () {
        std::stringstream sstr;
        std::function<std::string(Element*)> recursive =
            [&](Element * elem) -> std::string {
            return elem == nullptr ? ""
                                   : std::to_string(elem->value) +
                                         (elem->next == nullptr ? ""
                                                                : ", " + recursive(elem->next));
        };
        sstr << "[" << recursive(_first) << "]";
        return sstr.str();
    }

    inline unsigned int __len__ () { return _size; }

    void append(const int & value) {
        if (_first == nullptr) {
            _first = _last = new Element(value, nullptr);
        } else {
            _last->next = new Element(value, nullptr);
            _last = _last->next;
        }
        ++_size;
    }

    void insert(const unsigned int & idx, const int & value) {
        if (idx == 0) {
            _first = new Element(value, _first);
            ++_size;
        } else if (idx >= _size - 1) {
            append(value);
        } else {
            Element * prev = (*this)[idx-1];
            prev->next = new Element(value, prev->next);
            ++_size;
        }
    }

    int pop (int idx = -1) {
        idx = idx < 0 ? _size - 1 : idx;
        int value = 0;
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
            Element * oldFirst = _first;
            _first = _first->next;
            delete oldFirst;
        } else {
            Element * elemBefore = (*this)[idx-1];
            Element * elemAfter = elemBefore->next->next;
            value = elemBefore->next->value;
            delete elemBefore->next;
            elemBefore->next = elemAfter;
        }
        --_size;
        return value;
    }

    Element * operator [] (const unsigned int & idx) {
        if (! _size)
            throw py::index_error("empty list");
        else if (idx >= _size)
            throw py::index_error("list index is out of range");
        else if (! idx )
            return _first;
        else if (idx == (_size - 1))
            return _last;

        int i = idx;
        std::function<Element *(Element *)> recursive =
            [&](Element * itr) -> Element *
        {
            return (! --i) ? itr->next : recursive(itr->next);
        };
        return recursive(_first);

    }

    inline int __getitem__ (const unsigned int & idx) {
        return (*this)[idx]->value;
    }

    void __setitem__(const unsigned int & idx,
                     const int & value) {
        (*this)[idx]->value = value;
    }

private:
    size_t _size;
    Element * _first;
    Element * _last;
};


struct StlList : public std::list<int>
{
    StlList() : std::list<int>() {}
    StlList(const unsigned int & size) : std::list<int>(size, 0) {}

    std::string __repr__ () {
        std::stringstream sstr;
        sstr << "[";
        auto itr = this->begin();
        while (itr != this->end()) {
            sstr << std::to_string(*itr);
            if (std::next(itr, 1) != this->end())
                sstr << ", ";
            ++itr;
        }
        sstr << "]";
        return sstr.str();
    }

    inline void append(const int & value) {
        this->push_back(value);
    }

    inline void insert(const unsigned int & idx, const int & value) {
        std::list<int>::insert(std::next(this->begin(), idx), value);
    }

    inline int pop(int idx = -1) {
        idx = idx < 0 ? std::size(*this) - 1 : idx;
        if (idx >= std::size(*this) || ! std::size(*this))
            throw py::index_error("pop index out of range");

        auto itr = std::next(this->begin(), idx);
        const int value = *itr;
        std::list<int>::erase(itr);
        return value;
    }

    inline int __getitem__ (const unsigned int & idx) {
        return *std::next(this->begin(), idx);
    }

    inline void __setitem__ (const unsigned int & idx,
                             const int & value) {
        *std::next(this->begin(), idx) = value;
    }

    inline int __size__ () {
        return std::size(*this);
    }
};


DEFINE_MODULE(MODULE_NAME(list), m)
{
    py::class_<List>(m, "MyList")
        .def(py::init<>())
        .def(py::init<unsigned int &>())
        .def("__repr__", &List::__repr__)
        .def("append", &List::append)
        .def("__len__", &List::__len__)
        .def("__getitem__", &List::__getitem__)
        .def("__setitem__", &List::__setitem__)
        .def("insert", &List::insert)
        .def("pop", &List::pop, py::arg("idx") = -1)
        ;

    py::class_<StlList>(m, "StlList")
        .def(py::init<>())
        .def(py::init<unsigned int>())
        .def("__repr__", &StlList::__repr__)
        .def("append", &StlList::append)
        .def("__len__", &StlList::__size__)
        .def("__getitem__", &StlList::__getitem__)
        .def("__setitem__", &StlList::__setitem__)
        .def("insert", &StlList::insert)
        .def("pop", &StlList::pop, py::arg("idx") = -1)
        ;
}

