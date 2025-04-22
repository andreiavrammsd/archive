/*
 The C++ Programming Language, 4th Edition
 */

#include <ostream>
#include <cassert>
#include <sstream>

using std::istream;
using std::ostream;
using std::ostringstream;

class complex {
    double re, im;
public:
    constexpr complex(double r = 0, double i = 0) : re{r}, im{i} {}

    constexpr double real() const {
        return re;
    }

    constexpr double imag() const {
        return im;
    }

    void real(double r) {
        re = r;
    }

    void imag(double i) {
        im = i;
    }

    complex &operator+=(complex);

    complex &operator+=(double);

    // -=, *=, and /=
};

complex operator+(complex, complex);

complex operator+(complex, double);

complex operator+(double, complex);

// binary -, *, and /

complex operator-(complex); // unary minus
complex operator+(complex); // unary plus

bool operator==(complex, complex);

bool operator!=(complex, complex);

istream &operator>>(istream &, complex &); // input
ostream &operator<<(ostream &, complex); // output

complex acos(complex);

complex &complex::operator+=(complex c) {
    re += c.re;
    im += c.im;
    return *this;
}

complex &complex::operator+=(double d) {
    re += d;
    return *this;
}

complex operator+(complex a, complex b) {
    return a += b;
}

complex operator+(complex a, double b) {
    return {a.real() + b, a.imag()};
}

ostream &operator<<(ostream &os, complex c) {
    os << "(" << c.real() << ", " << c.imag() << ")";
    return os;
}

int main() {
    complex c0;
    assert(c0.real() == 0);
    assert(c0.imag() == 0);

    complex c1{1, 2};
    complex c2{3, 4};
    assert(c1.real() == 1);
    assert(c1.imag() == 2);
    assert(c2.real() == 3);
    assert(c2.imag() == 4);

    c2 += c1;
    assert(c1.real() == 1);
    assert(c1.imag() == 2);
    assert(c2.real() == 4);
    assert(c2.imag() == 6);

    c2 += 1.2;
    assert(c2.real() == 5.2);
    assert(c2.imag() == 6);

    c0 = c1 + c2;
    assert(c0.real() == 6.2);
    assert(c0.imag() == 8);

    c0 = c0 + 5;
    assert(c0.real() == 11.2);
    assert(c0.imag() == 8);

    ostringstream os;
    os << c0;
    assert(os.str() == "(11.2, 8)");

    c0.real(0);
    c0.imag(0);
    assert(c0.real() == 0);
    assert(c0.imag() == 0);
}
