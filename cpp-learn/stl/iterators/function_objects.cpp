/*
 The C++ Programming Language, 4th Edition
 */

#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>
#include <numeric>

/**
 * Many of the standard algorithms take function objects (or functions) as arguments to control the
 * way they work. Common uses are comparison criteria, predicates (functions returning bool), and
 * arithmetic operations. In <functional>, the standard library supplies a few common function
 * objects:
*/

template<typename T>
std::vector<T> expected(std::initializer_list<T> l);

int main() {
    std::vector<int> v{1, 2, 3, 0, 5};
    std::vector<int> v2{1, 0, 3, 0};
    std::vector<int> out;

    /**
     * Predicates
     */

    // equal_to
    assert(!equal(v.begin(), v.end(), v2.begin(), std::equal_to<>{}));

    // not_equal_to
    assert(!equal(v.begin(), v.end(), v2.begin(), std::not_equal_to<>{}));

    // greater_equal
    assert(equal(v.begin(), v.end(), v2.begin(), std::greater_equal<>{}));

    // less
    sort(v.begin(), v.end(), std::less<>{});
    assert(v == expected({0, 1, 2, 3, 5}));

    // greater
    sort(v.begin(), v.end(), std::greater<>{});
    assert(v == expected({5, 3, 2, 1, 0}));

    // logical_and
    transform(v.begin(), v.end(), v2.begin(), back_inserter(out), std::logical_and<>{});
    assert(out == expected({1, 0, 1, 0, 0}));

    v = {1, 0, 1};
    v2 = {0, 0, 1};

    // bit_and
    out.clear();
    transform(v.begin(), v.end(), v2.begin(), back_inserter(out), std::bit_and<>{});
    assert(out == expected({0, 0, 1}));

    // bit_or
    out.clear();
    transform(v.begin(), v.end(), v2.begin(), back_inserter(out), std::bit_or<>{});
    assert(out == expected({1, 0, 1}));

    // bit_xor
    out.clear();
    transform(v.begin(), v.end(), v2.begin(), back_inserter(out), std::bit_xor<>{});
    assert(out == expected({1, 0, 0}));

    /**
     * Arithmetic Operations
     */

    v = {1, 2, 3};
    v2 = {2, 1, 3};

    // plus
    assert(accumulate(v.begin(), v.end(), 0, std::plus<>{}) == 6);

    // minus
    assert(accumulate(v.begin(), v.end(), -3, std::minus<>{}) == -9);

    // multiplies
    assert(accumulate(v.begin(), v.end(), 2, std::multiplies<>{}) == 12);

    // divides
    assert(accumulate(v.begin(), v.end(), 12, std::divides<>{}) == 2);

    // modulus
    out.clear();
    transform(v.begin(), v.end(), v2.begin(), back_inserter(out), std::modulus<>{});
    assert(out == expected({1, 0, 0}));

    // negate
    out.clear();
    transform(v.begin(), v.end(), back_inserter(out), std::negate<>{});
    assert(out == expected({-1, -2, -3}));
}

template<typename T>
std::vector<T> expected(std::initializer_list<T> l) {
    return std::vector<T>{l};
}
