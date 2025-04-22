/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>

enum Day {
    sun, mon, tue, wed, thu, fri, sat
};

Day &operator++(Day &d) {
    return d = (sat == d) ? sun : static_cast<Day>(d + 1);
}

class complex {
    double re, im;
public:
    complex(double re, double im) : re(re), im(im) {}

    complex &operator+=(complex a) {
        re += a.re;
        im += a.im;
        return *this;
    }

    friend complex operator+(complex a, complex b) {
        return a += b;
    }

    complex &operator+=(double a) {
        re += a;
        return *this;
    }

    friend complex operator+(complex a, double b) {
        return {a.re + b, a.im};
    }

    friend bool operator==(complex a, complex b) {
        return a.re == b.re && a.im == b.im;
    }
};

int main() {
    auto day = Day::wed;

    assert(++day == thu);
    assert(++day == fri);
    assert(++day == sat);
    assert(++day == sun);
    assert(++day == mon);
    assert(++day == tue);
    assert(++day == wed);

    // Mixed-Mode Arithmetic
    complex a{2, 3};
    complex b{1, 1};

    complex c = a + b + 2.0;
    assert(c == complex(5, 4));
}
