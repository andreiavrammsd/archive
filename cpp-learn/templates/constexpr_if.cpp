/*
 Since C++17
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md

 Write code that is instantiated depending on a compile-time condition.
 */

#include <type_traits>

template<typename T>
constexpr bool isIntegral() {
    if constexpr (std::is_integral<T>::value) {
        return true;
    } else {
        return false;
    }
}

int main() {
    static_assert(isIntegral<int>());
    static_assert(isIntegral<char>());
    static_assert(!isIntegral<double>());
    struct S {
    };
    static_assert(!isIntegral<S>());
}