/*
 The C++ Programming Language, 4th Edition
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP20.md#designated-initializers
 */

#include <cstdlib>
#include <initializer_list>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <random>
#include <ostream>
#include <tuple>

template<typename T, size_t N>
struct array {
    explicit array(std::initializer_list<T> list) {
        std::copy(list.begin(), list.begin() + N, elem);
    }

    T *begin() noexcept {
        return elem;
    }

    const T *begin() const noexcept {
        return elem;
    };

    T *end() noexcept {
        return elem + N;
    }

    const T *end() const noexcept {
        return elem + N;
    }

    constexpr size_t size() noexcept {
        return N;
    }

    T &operator[](size_t n) {
        return elem[n];
    };

    const T &operator[](size_t n) const {
        return elem[n];
    }

    T *data() noexcept {
        return elem;
    }

    const T *data() const noexcept {
        return elem;
    }

private:
    T elem[N];
};

template<typename T>
struct Point {
    static_assert(std::is_arithmetic<T>(), "Accepts only numbers.");

    T x{}, y{};

    T sum() const {
        return x + y;
    }

    friend bool operator<(const Point<int> &x, const Point<int> &y);

    friend std::ostream &operator<<(std::ostream &, const Point<int> &);
};

bool operator<(const Point<int> &a, const Point<int> &b) {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

std::ostream &operator<<(std::ostream &os, const Point<int> &p) {
    os << "x = " << p.x << "; y = " << p.y;
    return os;
}

using Array = array<Point<int>, 3>; // array of 3 Points

struct Color {
    unsigned char r{};
    unsigned char g{};
    unsigned char b{};
};

struct Color2 {
    unsigned char r{};
    unsigned char g{};
    unsigned char b{};
} yellow, blue;

int main() {
    Color red{};
    static_assert(sizeof(red) == sizeof(char) * 3);
    red.r = 255;

    yellow.r = 255;
    yellow.g = 255;

    Array points{
            {1, 2},
            {3, 4},
            {5, 6},
            {5, 6},
    };

    // NumberOfFlags
    assert(points.size() == 3);

    // Range
    for (const auto &p : points) {
        std::cout << p.sum() << std::endl;
    }

    // Subscript
    assert(points[1].x == 3);
    points[0] = Point<int>{0, 0};
    points[1].x = 4;

    // Get all data
    assert(points.data()[1].x == 4);

    // Shuffle
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(points.begin(), points.end(), g);

    std::cout << std::endl << "Shuffled" << std::endl;
    for (const auto &p : points) {
        std::cout << p << std::endl;
    }

    // Sort
    std::sort(points.begin(), points.end());

    std::cout << std::endl << "Sorted" << std::endl;
    for (const auto &p : points) {
        std::cout << p << std::endl;
    }

    // Visibility
    // In C++, the name of a struct is entered into the scope in which it is declared [...]
    // Thus, the name of a C++ struct declared in an inner scope can hide the name in an outer scope.
    int x[99];
    static_assert(sizeof(x) == sizeof(int) * 99);

    { // scope
        struct x {
            int a;
        };
        static_assert(sizeof(x) == sizeof(int)); /* size of the array in C, size of the struct in C++ */
        static_assert(sizeof(struct x) == sizeof(int)); /* size of the struct */
    }

    // Designated initializers (C++20)
    struct A {
        int x{};
        int y{};
        int z{};
    };

    A a{
            .x = 1,
            .y = 2,
    };
    assert(a.x == 1);
    assert(a.y == 2);
    assert(a.z == 0);
}
