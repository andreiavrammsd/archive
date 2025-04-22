/*
 The C++ Programming Language, 4th Edition
 */

// [...] a value template parameter is a constant within the template so that an attempt to
// change the value of a parameter is an error.

#include <cstdlib>
#include <cassert>
#include <string>

template<typename T, int max>
class Buffer {
    T v[max];
public:
    Buffer(int) {
        // max = 1; // error: attempt to assign to template value argument
    }
};

template<typename T, size_t sz = 0>
class Vec {
public:
    T size() {
        return sz;
    }
};

template<typename Key, typename V, bool(*cmp)(const Key &, const Key &)>
class map {
public:
    map() = default;

    size_t size() const {
        return 0;
    }
};

bool insensitive(const std::string &, const std::string &) {
    // compare case insensitive (e.g., "hello" equals "HellO")
    return true;
}

template<typename Key, class V, typename Compare = std::less<Key>>
class stdmap {
public:
    stdmap() = default; // use the default comparison
    stdmap(Compare c) : cmp{c} {} // override the default
    size_t size() const {
        return 0;
    }

private:
    Compare cmp{}; // default comparison
};

int main() {
    Vec<int, 42> c1;
    Vec<int> c2;

    assert(c1.size() == 42);
    assert(c2.size() == 0);

    map<std::string, int, insensitive> m;
    assert(m.size() == 0);

    stdmap<std::string, int> stdm;
    assert(stdm.size() == 0);

    stdmap<std::string, int, std::greater<>> stdm2;
    assert(stdm2.size() == 0);
}
