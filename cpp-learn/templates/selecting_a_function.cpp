/*
 The C++ Programming Language, 4th Edition
 */

#include <iostream>
#include <type_traits>
#include <cassert>

struct X {
    std::string operator()(int x) {
        return "X" + std::to_string(x) + "!";
    }
};

struct Y {
    std::string operator()(int y) {
        return "Y" + std::to_string(y) + "!";
    }
};

int main() {
    typename std::conditional<(sizeof(int) > 4), X, Y>::type x;

    auto result = x(7);

    assert(result == (sizeof(int) > 4 ? "X7!" : "Y7!"));
}
