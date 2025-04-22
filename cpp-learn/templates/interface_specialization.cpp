/*
 The C++ Programming Language, 4th Edition
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 */

// Sometimes, a specialization is not an algorithmic optimization, but a modification of an interface
// (or even a representation). For example, the standard library complex uses specializations to adjust
// the set of constructors and the argument types for important operations for important specializations
// (such as complex<float> and complex<double> ). The general (primary) template (§25.3.1.1) looks like this:

#include <cassert>

template<typename T>
class complex {
    T re, im;
public:
    complex(const T &re = T{}, const T &im = T{}) : re{re}, im{im} {}

    complex(const complex &); // copy constructor

    template<typename X>
    complex(const complex<T> &); // conversion from complex<X> to complex<T>

    complex &operator=(const complex &);

    complex<T> &operator=(const T &);

    complex<T> &operator+=(const T &);

    // ...

    template<typename X>
    complex<T> &operator=(const complex<X> &);

    template<typename X>
    complex<X> &operator+=(const complex<X> &a) {
        re += a.re;
        im += a.im;
        return *this;
    }

    // ...

    constexpr T real() const {
        return re;
    }

    constexpr T imag() const {
        return im;
    }

    inline constexpr bool operator==(const complex<T> &x) const {
        return re == x.re && im == x.im;
    }
};

// Note that the scalar assignment operators take reference arguments. That's not efficient for floats,
// so complex<float> passes those by value:

template<>
class complex<float> {
    float re, im;
public:
    complex(float re = 0.0, float im = 0.0) : re{re}, im{im} {}

    complex<float> &operator=(float);

    complex<float> &operator+=(float r) {
        re += r;
        return *this;
    }

    complex<float> &operator=(const complex<float> &);

    inline constexpr bool operator==(const complex<float> &x) const {
        return re == x.re && im == x.im;
    }
};

// For complex<double>, that same optimization applies. In addition, conversions from complex<float>
// and complex<long double> are provided (as described in §23.4.6):

template<>
class complex<double> {
    double re{}, im{};
public:
    explicit constexpr complex(double re = 0.0, double im = 0.0) : re{re}, im{im} {}

    constexpr complex(const complex<float> &);

    explicit constexpr complex(const complex<long double> &c) {
        re = c.real();
        im = c.imag();
    }

    inline constexpr bool operator==(const complex<double> &x) const {
        return re == x.re && im == x.im;
    }
};

int main() {
    complex<int> ci{1, 2};
    ci += ci;
    assert(ci == complex<int>(2, 4));

    complex<float> cf;
    cf += 2;
    assert(cf == complex<float>(2, 0));

    complex<double> cd{complex<long double>{3, 3}};
    assert(cd == complex<double>(3, 3));

    // Template argument deduction for class templates (C++17)
    complex ci2{1, 2};
}
