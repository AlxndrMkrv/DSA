#include "../common.hxx"
#include <functional>

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
            throw nb::index_error();
        else if (idx == 0)
            return elem->value;
        else
            return _recursiveGetItem(elem->next, --idx);
    }

private:
    Element * _top;
};

NB_MODULE(list, m)
{
    nb::class_<List>(m, "List")
        .def(nb::init<>())
        .def("append", &List::append)
        .def("__len__", &List::__len__)
        .def("__getitem__", &List::__getitem__)
        ;
}
