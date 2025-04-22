/*
 Since C++14
 https://en.cppreference.com/w/cpp/14
 */

#include <cassert>

template<class T>
constexpr T pi = T(3.1415926535897932385L);  // variable template

template<class T>
constexpr T circular_area(T r) { // function template
    return pi<T> * r * r; // pi<T> is a variable template instantiation
}

int main() {
    assert(circular_area(2) == 12);
    static_assert(circular_area(2) == 12);
}
