/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>

struct Base {
    int b;

    Base(int b) : b{b} {}

    Base(const Base &) = default;
};

struct Derived : Base {
    int d;

    Derived(int d) : Base(d + 1), d{d} {}

    Derived(const Derived &) = default;
};

void naive(Base *p) {
    Base b2 = *p; // may slice: invokes Base::Base(const Base&)
    assert(b2.b == 2);

    auto d = reinterpret_cast<Derived *>(p);
    assert(d->d == 1);
    assert(d->b == 2);
}

class P {
public:
    int a{};
};

class C : public P {
public:
    int b{};
};

void func_ref(P &p) {
    assert(p.a == 1);

    auto c = reinterpret_cast<C &>(p);
    assert(c.a == 1);
    assert(c.b == 2);
}

void func_ptr(P *p) {
    assert(p->a == 1);

    auto c = reinterpret_cast<C *>(p);
    assert(c != nullptr);
    assert(c->a == 1);
    assert(c->b == 2);
}

void func_slice(P p) {
    assert(p.a == 1);

    auto c = reinterpret_cast<C &>(p);
    assert(c.a == 1);
    assert(c.b != 2);
}

int main() {
    Derived d{1};
    assert(d.d == 1);
    assert(d.b == 2);

    naive(&d);

    Base bb = d; // slices: invokes Base::Base(const Base&), not Derived::Derived(const Derived&)
    assert(bb.b == 2);

    auto bbd = reinterpret_cast<Derived *>(&bb);
    assert(bbd->d != 1);
    assert(bbd->b == 2);

    // The variables b2 and bb contain copies of the Base part of d, that is, a copy of d.b. The member d.d
    // is not copied. This phenomenon is called slicing.

    C c;
    c.a = 1;
    c.b = 2;
    func_ref(c);
    func_ptr(&c);
    func_slice(c);
}
