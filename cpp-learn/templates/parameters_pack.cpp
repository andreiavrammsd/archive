/*
 The C++ Programming Language, 4th Edition
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 */

#include <iostream>
#include <array>

template<typename T>
void print(T value) {
    std::cout << value;
}

template<typename T, typename... Args>
void print(T value, Args... args) {
    if (sizeof...(args) != 0) {
        std::cout << value;
        return print(args...);
    }
}

template<typename... Bases>
class X : public Bases ... {
public:
    X(const Bases &... b) : Bases(b)... {}

    constexpr size_t bases() const {
        return sizeof...(Bases);
    }
};

class Bx {
};

class By {
};

class Bz {
};

template<typename... Args>
constexpr auto sum(Args... args) {
    return (... + args);
}

int main() {
    print(1, 2, 3, 4, 5, 6);

    X<> x0;
    static_assert(x0.bases() == 0, "not 0");

    X<Bx> x1(Bx{});
    static_assert(x1.bases() == 1, "not 1");

    X<Bx, By> x2{Bx{}, By{}};
    static_assert(x2.bases() == 2, "not 2");

    X<Bx, By, Bz> x3(Bx{}, By{}, Bz{});
    static_assert(x3.bases() == 3, "not 3");

    // Folding expressions (C++17)
    static_assert(sum(1, 2, 3) == 6);
}
