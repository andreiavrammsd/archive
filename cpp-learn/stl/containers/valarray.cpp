/*
 The C++ Programming Language, 4th Edition
 */

#include <valarray>
#include <iostream>
#include <cassert>

bool equals(const std::valarray<int> &v, std::initializer_list<int> l) {
    if (v.size() != l.size()) {
        return false;
    }

    size_t i = 0;
    auto it = l.begin();

    while (it != l.end()) {
        if (*it != v[i]) {
            return false;
        }
        ++it;
        ++i;
    }

    return true;
}

int main() {
    std::valarray<int> a{1, 2, 3};
    std::valarray<int> b{4, 5, 6};

    auto sum = a + b;
    assert(equals(sum, {5, 7, 9}));

    b -= 2;
    assert(equals(b, {2, 3, 4}));

    auto a2 = a.shift(1);
    assert(equals(a2, {2, 3, 0}));

    a2 = a.cshift(1);
    assert(equals(a2, {2, 3, 1}));

    assert(a2.sum() == 6);
    assert(a2.max() == 3);
    assert(a2.min() == 1);

    a2 = a2.apply([](int i) -> int {
        return std::pow(i, 2);
    });
    assert(equals(a2, {4, 9, 1}));

    a2 = a2 << 2;
    assert(equals(a2, {16, 36, 4}));
}
