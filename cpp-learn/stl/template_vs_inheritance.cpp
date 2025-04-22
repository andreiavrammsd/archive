/*
 The C++ Programming Language, 4th Edition

 If the types in the interface to the generated or derived classes need to differ, templates have
 an advantage. To gain access to differing interfaces for derived classes through a base, we
 must use some form of explicit casting (§22.2).762

 If the implementations of generated or derived classes differ only through a parameter or
 differ only in a few special cases, templates have an advantage. Irregular implementations
 can be expressed through derived classes or specializations.

 If the actual types of objects used cannot be known at compile time, a class hierarchy is
 essential.

 If a hierarchical relationship is required between generated or derived types, hierarchies
 have an advantage. The base class provides a common interface. Conversions among tem-
 plate specializations must be explicitly defined by the programmer (§27.2.2).

 If explicit use of free store (§11.2) is undesirable, templates have an advantage.

 If run-time efficiency is at such a premium that inlining of operations is essential, templates
 should be used (because effective use of hierarchy requires the use of pointers or references,
 which inhibit inlining).
 */

#include <cassert>
#include <typeinfo>

// Template

struct A {
};
struct B {
};

template<typename X>
class Ct { // interface expressed in terms of parameter
    X mem{};
public:
    X f() {
        return {};
    }

    int g() {
        return 1;
    }

    void h(X) {

    }
};

template<>
class Ct<A> { // specialization (for A)
    A *mem{}; // the representation can differ from that of the primary template
public:
    A f() {
        return A{};
    }

    int g() {
        return 2;
    }

    void h(A) {}

    void k(int) {} // added functionality

    void test() {
        assert(mem == nullptr);
    }
};

// Inheritance (class hierarchy)

class X {
};

class Cx { // interface expressed in terms of types in scope
    X mem;
public:
    virtual X &f() = 0;

    virtual int g() = 0;

    virtual void h(X &) = 0;

    void test() {
        assert(typeid(mem) == typeid(X));
    }
};

class DA : public Cx { // derived class
public:
    X &f() override {
        static X x;
        return x;
    };

    int g() override {
        return 1;
    };

    void h(X &) override {

    }
};

class DB : public Cx { // derived class
    DB *p{}; // representation can be more extensive than what the base provides
public:
    X &f() override {
        static X x;
        return x;
    };

    int g() override {
        return 2;
    };

    void h(X &) override {

    }

    void k(int) {} // added functionality

    void test() {
        assert(p == nullptr);
    }
};

int main() {
    // Template
    Ct<A> cta; // specialization for A
    cta.f();
    assert(cta.g() == 2);
    cta.h(A{});
    cta.k(1);

    Ct<B> ctb; // specialization for B
    ctb.f();
    assert(ctb.g() == 1);
    ctb.h(B{});
    // ctb.k(1); // error

    // Inheritance (class hierarchy)
    Cx &cxa{*new DA}; // cxa is an interface to a DA
    cxa.f();
    assert(cxa.g() == 1);
    X x;
    cxa.h(x);

    if (auto cxb2 = dynamic_cast<DA *>(&cxa)) {
        // cxb2->k(1); error
        static_assert(true, "not DA");
        cxb2->f();
    } else {
        assert(false);
    };

    Cx &cxb{*new DB}; // cxb is an interface to a DB
    cxb.f();
    assert(cxb.g() == 2);
    cxb.h(x);

    if (auto cxb2 = dynamic_cast<DB *>(&cxb)) {
        cxb2->k(1);
        static_assert(true, "not DB");
    } else {
        assert(false);
    };
}
