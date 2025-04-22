/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <typeinfo>

// A friend must be previously declared in an enclosing scope or defined in the non-class scope
// immediately enclosing the class that is declaring it to be a friend . Scopes outside the innermost
// enclosing namespace scope are not considered for a name first declared as a friend (§iso.7.3.1.2).
class C1 {
}; //  will become friend of N::C

void f1(); // will become friend of N::C

namespace N {
    class C2 {
    }; // will become friend of C

    void f2() {} // will become friend of C

    class C {
        int x{};
    public:
        friend class C1; // OK (previously defined)
        friend void f1();

        friend class C3; // OK (defined in enclosing namespace)
        friend void f3();

        friend class C4; // First declared in N and assumed to be in N
        friend void f4();
    };

    class C3 {
    }; // friend of C
    void f3() {
        C x;
        x.x = 1; // OK: friend of C
    }
} // namespace N

class C4 {
}; // not friend of N::C

void f4() {
    N::C x;
    assert(typeid(x) == typeid(N::C));
    // x.x = 1; // error : x is private and f4() is not a friend of N::C
}

class X {
private:
    int x;
public:
    X(int x) : x{x} {}

    int m1(); // member
    int m2() const;

    friend int ff1(X &); // friend, not member
    friend int ff2(const X &);

    friend int ff3(X);
};

void g() {
    // 99.m1(); // error: X(99).m1() not tried
    // 99.m2(); // error: X(99).m2() not tried
}

int ff1(X &x) {
    x.x = 2;
    return x.x;
}

void h() {
    X x{1};
    assert(ff1(x) == 2);
}

int main() {
    N::f2();
    N::f3();
    f4();

    h();
}
