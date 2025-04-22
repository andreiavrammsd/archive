/*
 The C++ Programming Language, 4th Edition

 For the purposes of copying, a base is just a member: to copy an object of a derived class you have
 to copy its bases.
 */

#include <cassert>

struct B1 {
    B1() = default;

    B1(const B1 &) = default;
};

struct B2 {
    B2(int b) : b{b} {};

    B2(const B2 &) = default;

    bool operator==(const B2 &b2) const {
        return b == b2.b;
    }

private:
    int b{};
};

struct D : B1, B2 {
    D(int i) : B1{}, B2{i}, m1{}, m2{2 * i} {}

    D(const D &a) : B1{a}, B2{a}, m1{a.m1}, m2{a.m2} {}

    B1 m1;
    B2 m2;

    bool operator==(const D &d) const {
        return m2 == d.m2;
    }
};

int main() {
    D d{1}; // construct with int argument
    D dd{d}; // copy construct

    assert(d == dd);
}
