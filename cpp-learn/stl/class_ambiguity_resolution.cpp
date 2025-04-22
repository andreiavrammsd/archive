/*
 The C++ Programming Language, 4th Edition
 */

#include <string>
#include <cassert>

// Two base classes may have member functions with the same name. For example:
struct Debug_info {
    std::string info;
};

class Satellite {
   public:
    virtual Debug_info get_debug() { return {"s"}; }
};

class Displayed {
   public:
    virtual Debug_info get_debug() { return {"d"}; }
};

// When a Comm_sat is used, these functions must be disambiguated. This can be done simply by
// qualifying a member name by its class name:
class Comm_sat : public Satellite, public Displayed {
};

void f(Comm_sat &cs)
{
    Debug_info di;
    // di = cs.get_debug(); // error: ambiguous
    di = cs.Satellite::get_debug();  // OK
    di = cs.Displayed::get_debug();  // OK
}

// However, explicit disambiguation is messy, so it is usually best to resolve such problems by defining
// a new function in the derived class:
class Comm_sat2 : public Satellite, public Displayed {
   public:
    Debug_info get_debug() override
    {  // override Comm_sat::get_debug() and Displayed::get_debug()
        Debug_info d1 = Satellite::get_debug();
        Debug_info d2 = Displayed::get_debug();
        return merge_info(d1, d2);
    }

   private:
    Debug_info merge_info(const Debug_info &d1, const Debug_info &d2) { return {d1.info + d2.info}; }
};

// A function declared in a derived class overrides all functions of the same name and type in its base
// classes. Typically, that is exactly the right thing to do because it is generally a bad idea to use the
// same name for operations with different semantics in a single class. The ideal for virtual is for a
// call to have the same effect independently of which interface was used to find the function (§20.3.2).

int main()
{
    Comm_sat2 comm;
    assert(comm.get_debug().info == "sd");
}
