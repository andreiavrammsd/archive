/*
 * https://www.youtube.com/watch?v=UNJrgsQXvCA
 */

#include <cstddef>
#include <iostream>

#define show(x) std::cout << #x << " = " << x << '\n';

class A {
public:
    char b;
    int a;
};

class B {
public:
    int a;
    char b;
};

class C {
public:
    int a;
    char b;
    char c;
};

class D {
public:
    char b;
    int a;
    char c;
};

int main() {
    static_assert(sizeof(A) == sizeof(B));


    show(sizeof(A));
    show(offsetof(A, a));
    show(offsetof(A, b));

    std::cout << '\n';

    show(sizeof(B));
    show(offsetof(B, a));
    show(offsetof(B, b));

    std::cout << '\n';

    static_assert(sizeof(C) != sizeof(D));

    show(sizeof(C));
    show(offsetof(C, a));
    show(offsetof(C, b));
    show(offsetof(C, c));

    std::cout << '\n';

    show(sizeof(D));
    show(offsetof(D, a));
    show(offsetof(D, b));
    show(offsetof(D, c));
}
