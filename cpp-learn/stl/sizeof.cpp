#include <string>
#include <cassert>

template<typename T>
long size_of(T v) {
    auto a = &v;
    ++a;

    return long(a) - long(&v);
}

struct S {
    int a;
};

int main() {
    int i = 1;
    std::string str = "a";
    S s{1};

    assert(size_of(i) == sizeof(i));
    assert(size_of(str) == sizeof(str));
    assert(size_of(s) == sizeof(s));
}
