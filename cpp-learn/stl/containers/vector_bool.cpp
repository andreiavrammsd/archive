/*
 The C++ Programming Language, 4th Edition
 */

#include <algorithm>
#include <functional>

/**
 * The vector<bool> from <vector> is a specialization of vector (§31.4) providing
 * compact storage of bits (bool).
 *
 * The similarity to bitset_ is obvious, but, unlike bitset_ but like vector<T>,
 * vector<bool> has an allocator and can have its size changed.
 *
 * As in a vector<T>, elements of a vector<bool> with higher indices have higher addresses:
 * position:        0 1 2 3 4 5 6 7 8 9
 * vector<bool>:    1 1 1 1 0 1 1 1 0 1
 *
 * Use vector<bool> as you would any other vector<T>, but expect operations on a single
 * bit to be less efficient than the equivalent operations on a vector<char>.
 */

#include <cassert>
#include <vector>

int main()
{
    std::vector<bool> bits1{true, false, false, true};
    std::vector<bool> bits2{false, false, true, true};
    std::vector<bool> expected{true, false, true, false};
    std::vector<bool> out;

    std::transform(bits1.begin(), bits1.end(), bits2.begin(), back_inserter(out), std::bit_xor<>{});

    assert(out == expected);
}
