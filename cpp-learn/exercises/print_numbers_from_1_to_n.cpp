/*
 Print numbers from 1 to n.
 */

#include <iostream>
#include <array>

static const size_t n = 10;

// Array initialization
static size_t si;

struct S {
    S() {
        std::cout << ++si << " ";
    }
};

// Template
template<size_t N>
void print() {
    std::cout << N << " ";
    print<N + 1>();
}

template<>
void print<n>() {
    std::cout << n;
}

int main() {
    // For
    for (size_t i = 1; i <= n; i++) {
        std::cout << i << " ";
    }

    std::cout << std::endl;

    // Array initialization
    std::array<S, n> arr{};
    static_assert(!arr.empty());

    std::cout << std::endl;

    // Template
    print<1>();
}
