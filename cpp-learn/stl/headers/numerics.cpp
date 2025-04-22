/*
 The C++ Programming Language, 4th Edition
 */

#include <complex> // Complex numbers and operations
#include <valarray> // Numeric vectors and operations
#include <numeric> // Generalized numeric operations
#include <cmath> // Standard mathematical functions
#include <cstdlib> // C-style random numbers
#include <random> // Random number generators

#include <cassert>
#include <chrono>
#include <iostream>

int main() {
    // Complex numbers and operations
    std::complex<double> cmp{1, 2};
    assert(cmp.real() == 1);
    assert(cmp.imag() == 2);

    cmp *= 2;
    assert(cmp.real() == 2);
    assert(cmp.imag() == 4);

    // Numeric vectors and operations
    std::valarray<double> varr{2, 3};
    assert(varr.max() == 3);

    // Generalized numeric operations
    std::vector<int> numbers = {1, 2, 3};
    auto result = std::accumulate(numbers.begin(), numbers.end(), 2, std::multiplies<>{});
    assert(result == 12);

    std::partial_sum(numbers.begin(), numbers.end(), numbers.begin());
    assert(numbers == (std::vector<int>{1, 3, 6}));

    assert(std::gcd(12, 4) == 4); // since C++17
    assert(std::lcm(12, 4) == 12); // since C++17

    // Standard mathematical functions
    assert(std::pow(2, 5) == 32);
    assert(std::cos(0) == 1);

    // Standard mathematical functions (C++17)
    // https://en.cppreference.com/w/cpp/numeric/special_functions
    assert(std::beta(1, 2) == 0.5);
    std::cout << std::expint(10.0) << '\n';
    assert(std::hypot(1, 2) == std::abs(std::complex<double>(1, 2)));

    // C-style random numbers
    srandom(std::chrono::steady_clock::now().time_since_epoch().count());
    int min = 1;
    int max = 5;
    std::cout << (rand() % (max - min)) + min << '\n';

    // Random number generators
    auto random = [] {
        std::random_device rnd{};
        std::default_random_engine eng{rnd()};
        std::uniform_int_distribution<> dist{1, 5};
        return dist(eng);
    };
    std::cout << random() << '\n';
}
