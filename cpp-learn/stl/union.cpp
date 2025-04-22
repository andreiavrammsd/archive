/*
 The C++ Programming Language, 4th Edition
 */

#include <iostream>
#include <cassert>

enum Type {
    str,
    num,
};

union Value {
    char *s;
    int i;
};

struct Entry {
    char *name;
    Type t;
    Value v;
};

void f(Entry *p) {
    if (p->t == num) {
        std::cout << p->v.i << std::endl;
        return;
    }

    std::cout << p->v.s << std::endl;
}

class X {
public:
    union {
        int a;
        char b;
    };
};

union WithFunc {
    int a, b;

    WithFunc() = default;

    static WithFunc withA(int v) {
        auto u = WithFunc{};
        u.a = v;
        return u;
    }


    static WithFunc withB(int v) {
        auto u = WithFunc{};
        u.b = v;
        return u;
    }

    ~WithFunc() = default;
};

int main() {
    auto e1 = new Entry{};
    e1->t = str;
    e1->v.s = (char *) "abc";
    f(e1);

    auto e2 = new Entry{};
    e2->t = num;
    e2->v.i = 1;
    f(e2);

    auto e3 = new Entry{};
    e3->v.s = (char *) "xyz";
    f(e3);

    // Anonymous union
    X x{};
    static_assert(sizeof(x.a) == sizeof(int));

    // Union with functions
    auto wfa = WithFunc::withA(1);
    assert(wfa.a == 1);

    auto wfb = WithFunc::withB(2);
    assert(wfb.b == 2);
}
