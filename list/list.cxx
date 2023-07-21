#include "../common.hxx"
#include <functional>
#include <list>

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
    List() : _top(nullptr) {}
    List(unsigned int & n) : _top(nullptr) {
        for (auto i = 0; i < n; ++i)
            this->append(0);
    }
    ~List() {
        _recursiveDelete(_top);
    }

    unsigned int __len__ () {
        unsigned int counter = 0;
        std::function<unsigned int(Element*)> recursive =
            [&](Element * elem) {
                ++counter;
                return elem == nullptr ? --counter
                                       : recursive(elem->next);};
        return recursive(_top);
    }

    void append(const int & value) {
        _top = new Element(value, _top);
    }

    int __getitem__(unsigned int & idx) {
        return _recursiveGetItem(_top, idx);
    }

private:
    void _recursiveDelete (Element * elem) {
        if (elem != nullptr) {
            _recursiveDelete(elem->next);
            delete elem;
        }
    }

    int _recursiveGetItem (Element * elem, unsigned int & idx) {
        if (elem == nullptr)
            throw index_error("asdf");
        else if (idx == 0)
            return elem->value;
        else
            return _recursiveGetItem(elem->next, --idx);
    }

private:
    Element * _top;
};


struct StlList : public std::list<int>
{
    StlList() : std::list<int>() {}
    StlList(const unsigned int & size) : std::list<int>(size, 0) {}

    void append(const int & value) {
        this->push_back(value);
    }

    int __getitem__ (const unsigned int & i) {
        return *std::next(this->begin(), i);
    }
};


DEFINE_MODULE(MODULE_NAME(list), m)
{
    py::class_<List>(m, "MyList")
        .def(py::init<>())
        .def(py::init<unsigned int &>())
        .def("append", &List::append)
        .def("__len__", &List::__len__)
        .def("__getitem__", &List::__getitem__)
        ;

    py::class_<StlList>(m, "StlList")
        .def(py::init<>())
        .def(py::init<unsigned int>())
        .def("append", &StlList::append)
        .def("__len__", &StlList::size)
        .def("__getitem__", &StlList::__getitem__)
        ;
}

