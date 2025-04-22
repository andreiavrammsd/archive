/*
 The C++ Programming Language, 4th Edition
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 */

#include <cassert>

enum Type {
    str,
    num,
};

enum class Color { // or enum struct
    yellow = 1,
    black = 2,
};

int main() {
    auto t = Type::str;
    assert(t == 0);

    t = num;
    assert(t == 1);

    auto c = Color::black;
    assert(c != Color::yellow);

    // Direct list initialization of enums (C++17)
    // Enums can now be initialized using braced syntax.
    enum byte : unsigned char {
    };
    byte b{0};
    assert(b == 0);
    // byte c{-1}; // error
}
