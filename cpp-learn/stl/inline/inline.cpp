/*
 https://en.cppreference.com/w/cpp/17
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 */

#include <cassert>

#include "example.h"

int a();

int b();

struct S {
    int x;
};
inline S x1 = S{321};

int main() {
    /**
     * inline specifier
     *
     * A function defined entirely inside a class/struct/union definition, whether it's a member
     * function or a non-member friend function, is implicitly an inline function.
     *
     * A function declared constexpr is implicitly an inline function.
     *
     * A deleted function is implicitly an inline function: its (deleted) definition can appear
     * in more than one translation unit.
     *
     * The inline specifier, when used in a decl-specifier-seq of a variable with static storage
     * duration (static class member or namespace-scope variable), declares the variable to be
     * an inline variable.
     *
     * A static member variable (but not a namespace-scope variable) declared constexpr is
     * implicitly an inline variable.
     *
     * Inline variables eliminate the main obstacle to packaging C++ code as header-only libraries.
     */

    assert(counter == 0);

    assert(a() == 3);
    assert(counter == 1);

    assert(b() == 7);
    assert(counter == 2);

    // Inline variables (C++17)
    assert(x1.x == 321);
}
