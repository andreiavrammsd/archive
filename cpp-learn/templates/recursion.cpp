/*
 The C++ Programming Language, 4th Edition
 */

// factorial
template<int N>
struct Fac {
    static const int value = N * Fac<N - 1>::value;
};

template<>
struct Fac<1> {
    static const int value = 1;
};

constexpr int fac(int i) {
    return (i < 2) ? 1 : i * fac(i - 1);
}

// fibonacci
template<int N>
struct Fib {
    static constexpr int value = Fib<N - 1>::value + Fib<N - 2>::value;
};

template<>
struct Fib<0> {
    static const int value = 0;
};

template<>
struct Fib<1> {
    static const int value = 1;
};

int main() {
    constexpr int x7 = Fac<7>::value;
    static_assert(x7 == 5040, "7! != 5040");

    static_assert(fac(7) == 5040, "7! != 5040");

    constexpr int f5 = Fib<5>::value;
    static_assert(f5 == 5, "fib(5) != 5");
}
