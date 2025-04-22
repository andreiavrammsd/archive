/*
 * https://lemire.me/blog/2020/04/16/rounding-integers-to-even-efficiently/
 */

#include <bitset>
#include <cassert>
#include <vector>
#include <numeric>
#include <algorithm>

bool is_even_and_bitset(unsigned long num) {
    static const size_t sz = sizeof(int);
    static const std::bitset<sz> d{1};
    std::bitset<sz> n{num};
    return (n & d).none();
}

bool is_even_and(unsigned num) {
    static const unsigned d = 1;
    return (num & d) == 0;
}

int main() {
    std::vector<int> numbers(100);
    std::iota(numbers.begin(), numbers.end(), -50);

    std::for_each(numbers.cbegin(), numbers.cend(), [](int i) {
        auto expected = (i % 2 == 0);
        assert(is_even_and_bitset(i) == expected);
        assert(is_even_and(i) == expected);
    });
}
