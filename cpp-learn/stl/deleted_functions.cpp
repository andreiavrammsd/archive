/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <cstddef>
#include <typeinfo>

struct Z {
    explicit Z(double d) : d(d) {}; // can initialize with a double
    Z(int) = delete; // but not with an integer
    double d;
};

class Not_on_stack {
public:
    ~Not_on_stack() = delete;
};

class Not_on_free_store {
public:
    void *operator new(size_t) = delete;
};

int main() {
    // Z z1{1}; // error: Z(int) deleted

    Z z2{1.0}; // OK
    assert(z2.d == 1.0);

    // Not_on_stack v1; // error : can’t destroy
    Not_on_free_store v2; // OK
    assert(typeid(v2) == typeid(Not_on_free_store));

    Not_on_stack *p1 = new Not_on_stack; // OK
    assert(p1 != nullptr);
    // Not_on_free_store *p2 = new Not_on_free_store; // error : can't allocate
}
