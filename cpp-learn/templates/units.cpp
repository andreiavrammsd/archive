/*
 The C++ Programming Language, 4th Edition
 */

#include <string>
#include <cassert>
#include <ostream>
#include <sstream>

template<int M, int K, int S>
struct Unit {
    enum {
        m = M,
        kg = K,
        s = S,
    };
};

using M = Unit<1, 0, 0>; // meters
using Kg = Unit<0, 1, 0>; // kilograms
using S = Unit<0, 0, 1>; // seconds
using MpS = Unit<1, 0, -1>; // meters per second (m/s)
using MpS2 = Unit<1, 0, -2>; // meters per square seconds (m/(s*s))

template<typename U1, typename U2>
struct Uplus {
    using type = Unit<U1::m + U2::m, U1::kg + U2::kg, U1::s + U2::s>;
};

template<typename U1, typename U2>
using Unit_plus = typename Uplus<U1, U2>::type;

template<typename U1, typename U2>
struct Uminus {
    using type = Unit<U1::m - U2::m, U1::kg - U2::kg, U1::s - U2::s>;
};

template<typename U1, typename U2>
using Unit_minus = typename Uminus<U1, U2>::type;

template<typename U>
struct Quantity {
    long double val;

    explicit constexpr Quantity(long double d) : val{d} {}
};

// Computations
template<typename U>
Quantity<U> operator+(Quantity<U> x, Quantity<U> y) { // same dimension
    return Quantity<U>{x.val + y.val};
}

template<typename U>
Quantity<U> operator-(Quantity<U> x, Quantity<U> y) { // same dimension
    return Quantity<U>{x.val - y.val};
}

template<typename U1, typename U2>
Quantity<Unit_plus<U1, U2>> operator*(Quantity<U1> x, Quantity<U2> y) {
    return Quantity<Unit_plus<U1, U2>>{x.val * y.val};
}

template<typename U1, typename U2>
Quantity<Unit_minus<U1, U2>> operator/(Quantity<U1> x, Quantity<U2> y) {
    return Quantity<Unit_minus<U1, U2>>{x.val / y.val};
}

// Variants to arithmetic operations
template<typename U>
Quantity<U> operator*(Quantity<U> x, double y) {
    return Quantity<U>{x.val * y};
}

template<typename U>
Quantity<U> operator*(double x, Quantity<U> y) {
    return Quantity<U>{x * y.val};
}

// Unit literals
constexpr Quantity<M> operator "" _m(long double d) { return Quantity<M>{d}; }

constexpr Quantity<M> operator "" _m(unsigned long long d) { return Quantity<M>{static_cast<long double>(d)}; }

constexpr Quantity<Kg> operator "" _kg(long double d) { return Quantity<Kg>{d}; }

constexpr Quantity<Kg> operator "" _kg(unsigned long long d) { return Quantity<Kg>{static_cast<long double>(d)}; }

constexpr Quantity<S> operator "" _s(long double d) { return Quantity<S>{d}; }

constexpr Quantity<S> operator "" _s(unsigned long long d) { return Quantity<S>{static_cast<long double>(d)}; }

constexpr Quantity<M> operator "" _km(long double d) { return Quantity<M>{1000 * d}; }

constexpr Quantity<Kg> operator "" _g(long double d) { return Quantity<Kg>{d / 1000}; }

constexpr Quantity<Kg> operator "" _mg(long double d) { return Quantity<Kg>{d / 1000000}; } // milligram

constexpr Quantity<S> operator "" _ms(long double d) { return Quantity<S>{d / 1000}; } // milliseconds

constexpr Quantity<S> operator "" _us(long double d) { return Quantity<S>{d / 1000000}; } // microseconds

constexpr Quantity<S> operator "" _ns(long double d) { return Quantity<S>{d / 1000000000}; } // nanoseconds

// Utility functions
template<typename U>
Quantity<Unit_plus<U, U>> square(Quantity<U> x) {
    return Quantity<Unit_plus<U, U>>{x.val * x.val};
}

template<typename U>
bool operator==(Quantity<U> x, Quantity<U> y) {
    return x.val == y.val;
}

template<typename U>
bool operator!=(Quantity<U> x, Quantity<U> y) {
    return x.val != y.val;
}

std::string suffix(int u, const char *x) {
    std::string suf;
    if (u) {
        suf += x;

        if (1 < u) {
            suf += '0' + u;
        }

        if (u < 0) {
            suf += '-';
            suf += '0' - u;
        }
    }
    return suf;
}

template<typename U>
std::ostream &operator<<(std::ostream &os, Quantity<U> v) {
    return os << v.val << suffix(U::m, "m") << suffix(U::kg, "kg") << suffix(U::s, "s");
}

int main() {
    // Declare
    Quantity<M> x{10.5}; // x is 10.5 meters
    assert(x.val == 10.5);

    Quantity<S> y{2}; // y is 2 seconds
    assert(y.val == 2);

    // Operations
    Quantity<MpS> the_speed{10};

    auto double_speed = Quantity<Unit<0, 0, 0>>{2} * the_speed;
    assert(double_speed.val == 20);

    double_speed = 3 * the_speed;
    assert(double_speed.val == 30);

    // Notation
    /*
    auto distance = Quantity<M>{10}; // 10 meters
    auto time = Quantity<S>{20}; // 20 seconds
    auto speed = distance/time; // .5 m/s (meters per second)

     vs

    auto distance = 10.0; // 10 meters
    double time = 20; // 20 seconds
    auto speed = distance/time; // .5 m/s (meters per second)

     vs
     */

    auto distance = 10_m; // 10 meters
    auto time = 20_s; // 20 seconds
    auto speed = distance / time; // .5 m/s (meters per second)

    // Comparison
    // speed == 20; // error: 20 is dimensionless
    // speed == distance; // error: can't compare m to m/s
    assert(speed == 10_m / 20_s);
    assert(1000.0_g == 1_kg);

    // Stream
    Quantity<MpS2> acceleration = distance / square(time); // MpS2 means m/(s*s)

    std::stringstream s;
    s << "speed==" << speed << " acceleration==" << acceleration;
    assert(s.str() == "speed==0.5ms-1 acceleration==0.025ms-2");
}
