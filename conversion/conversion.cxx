#include "../common.hxx"
#include <nanobind/stl/unique_ptr.h>
#include <nanobind/stl/shared_ptr.h>

struct Data
{
    int field;
};

Data data;
Data * get_data_by_ptr() { return &data; }
Data & get_data_by_ref() { return data; }

//std::unique_ptr<Data, nb::deleter<Data>> get_data_by_uptr() { return std::unique_ptr<Data, nb::deleter<Data>>(&data); }
//std::shared_ptr<Data> get_data_by_sptr() { return std::shared_ptr<Data>(&data); }


NB_MODULE(conversion, m) {
    nb::class_<Data>(m, "Data").def_rw("field", &Data::field);

    /* nb::rv_policy tells the library what to do with object returned by reference/pointer
     *   - take_ownership: call 'delete' on C++ side when object deleted on python side.
     *       Will cause "free(): invalid pointer" with local/global variables.
     *       Useful only when object created by 'new' and not needed by c++ side;
     *   - copy: copy-construct new object for python side.
     *       Both sides take care about their copies;
     *   - move: move-construct new object for python side.
     *       Like 'copy' but keep in mind that c++'s part loses content and
     *       become useless;
     *   - reference: no copy, no transfered ownership, no called 'delete'.
     *       C++ side takes all responsibility for returned object;
     *   - reference_internal: like 'reference' but ensures that owner of the
     *       object won't be garbage collected until the object still in use
     *   - none: refuses to cast if C++ don't have corresponding Python object
     *   - automatic: 'take_ownership' to pointer, 'move' for rvalue reference
     *       and 'copy' for lvalue reference
     *   - automatic_reference: like 'automatic' but with 'reference' for pointers
     */

    // set 'reference' policy to overcome 'take_ownership'
    m.def("get_data_by_ptr", &get_data_by_ptr, nb::rv_policy::reference); // will give same id() by every call
    // set 'reference' policy to overcome 'copy'
    m.def("get_data_by_ref", &get_data_by_ref, nb::rv_policy::reference); // will give different id()'s


    // shared_ptr and unique_ptr are a tricky things that must be used with cautioness
    //m.def("get_data_by_uptr", &get_data_by_uptr);

    //
    //m.def("get_data_by_sptr", &get_data_by_sptr);

}


