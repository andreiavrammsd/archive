/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <algorithm>

struct A {
    int v;
    int *w;
};

int main() {
    int a[4] = {1, 2, 3, 4};
    A x{1, a};

    int b[4] = {5, 6, 7};
    A y{3, b};

    assert(x.v == 1);
    assert(x.w == a);
    assert(y.v == 3);
    assert(y.w == b);

    std::swap(x, y);

    assert(x.v == 3);
    assert(x.w == b);
    assert(y.v == 1);
    assert(y.w == a);
}
