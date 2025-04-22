/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 https://www.fluentcpp.com/2017/02/16/custom-comparison-equality-equivalence-stl/
 */

#include <algorithm>

#include <vector>
#include <cassert>
#include <string>

/**
 * includes
 * set_difference
 * set_intersection
 * set_symmetric_difference
 * set_union
 */

template<typename T>
std::vector<T> expected(std::initializer_list<T>);

int main() {
    std::vector<int> set1 = {1, 2, 3, 4, 5}; // sorted range
    std::vector<int> set2 = {1, 2, 3, 5}; // sorted range
    std::vector<int> out;

    // includes
    assert(includes(set1.begin(), set1.end(), set2.begin(), set2.end()));

    // set_union
    out.clear();
    set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(out));
    assert(out == expected({1, 2, 3, 4, 5}));

    // set_intersection
    out.clear();
    set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(out));
    assert(out == expected({1, 2, 3, 5}));

    // set_difference
    out.clear();
    set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(out));
    assert(out == expected({4}));

    std::vector<std::pair<int, std::string>> pairs_a{{1, "abc"},
                                                     {2, "bcd"}};
    std::vector<std::pair<int, std::string>> pairs_b{{1, "cde"},
                                                     {3, "efg"}};
    std::vector<std::pair<int, std::string>> pairs_out;

    auto compareFirst = [](const std::pair<int, std::string> &p1, const std::pair<int, std::string> &p2) {
        return p1.first < p2.first; // equality vs equivalence
    };

    set_difference(pairs_a.begin(), pairs_a.end(),
                   pairs_b.begin(), pairs_b.end(),
                   std::back_inserter(pairs_out),
                   compareFirst);
    assert(pairs_out == (expected<std::pair<int, std::string>>({{2, "bcd"}})));

    // set_symmetric_difference
    out.clear();
    set_symmetric_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(out));
    assert(out == expected({4}));
}

template<typename T>
std::vector<T> expected(std::initializer_list<T> l) {
    return std::vector<T>{l};
}
