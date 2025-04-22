/*
 The C++ Programming Language, 4th Edition
 */

#include <initializer_list>
#include <iostream>
#include <vector>
#include <cassert>

// new and delete
class X {
public:
    int x;
};

// typeid() and type_info
template<typename T>
bool isint(T t) {
    return typeid(t) == typeid(int);
}

// initializer_list Support
void f(std::initializer_list<int> lst) {
    const int *p = lst.begin();

    for (size_t i = 0; i < lst.size(); ++i) {
        std::cout << p[i] << '\n';
    }
    std::cout << '\n';

    for (const auto &x : lst) {
        std::cout << x << '\n';
    }
    std::cout << '\n';
}

// Range-for Support
void f2(const std::vector<int> &vec) {
    for (auto i = std::begin(vec); i != std::end(vec); ++i) {
        std::cout << *i << '\n';
    }
    std::cout << '\n';

    for (auto i = vec.begin(); i != vec.end(); ++i) {
        std::cout << *i << '\n';
    }
    std::cout << '\n';
}

int main() {
    auto x = new X{1};
    assert(x->x == 1);
    delete x;
    assert(x->x != 1);

    assert(isint(1));
    assert(!isint(1.0));

    f({1, 2, 3});

    f2({1, 2, 3});
}
