/*
 The C++ Programming Language, 4th Edition
 */

#include <vector>
#include <string>
#include <cassert>

struct Bad_index : std::exception {
    explicit Bad_index(size_t i) : i{i} {

    }

    size_t i{};
};

template<class T>
struct Vector : std::vector<T> {
    using std::vector<T>::vector; // inherit constructors

    T &operator[](size_t i) {
        check(i);
        return this->data()[i];
    }

    void check(size_t i) {
        if (this->size() - 1 < i) {
            throw Bad_index(i);
        }
    }
};

struct B1 {
    B1(int) {}
};

struct D1 : B1 {
    using B1::B1; // implicitly declares D1(int)
    std::string s; // string has a default constructor
    int x{}; // note: x is initialized
};

int main() {
    // Vector, inherit constructors
    Vector<int> v{1, 2};
    assert(v[0] == 1);
    assert(v[1] == 2);

    try {
        v[2];
        assert(false);
    } catch (const Bad_index &e) {
        assert(e.i == 2);
    }

    v[0] = 2;
    assert(v[0] == 2);

    // In-class member initializer
    D1 d{6}; // d.x is zero
    assert(d.x == 0);
}
