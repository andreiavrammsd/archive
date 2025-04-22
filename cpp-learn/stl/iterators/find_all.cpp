/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <vector>
#include <iostream>
#include <list>

template<typename T>
using Iterator = typename T::iterator;

template<typename C, typename V>
std::vector<Iterator<C>> find_all(C &c, V v);

int main() {
    std::string m{"Mary had a little lamb"};
    assert(find_all(m, 'a').size() == 4);
}

template<typename C, typename V>
std::vector<Iterator<C>> find_all(C &c, V v) {
    std::vector<Iterator<C>> res{};

    for (auto p = c.begin(); p != c.end(); ++p) {
        if (*p == v) {
            res.push_back(p);
        }
    }

    return res;
}
