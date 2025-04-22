/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <algorithm>

#include <vector>
#include <cassert>
#include <functional>

/**
 * lower_bound
 * upper_bound
 * binary_search
 * equal_range
 */

int main() {
    std::vector<int> nums = {1, 2, 3, 89, 99}; // sorted range

    // lower_bound
    auto p = lower_bound(nums.begin(), nums.end(), 3);
    assert(p == nums.begin() + 2);
    assert(*p == 3);

    nums = {99, 89, 3, 2, 1};
    p = lower_bound(nums.begin(), nums.end(), 89, std::greater<>{});
    assert(p == nums.begin() + 1);
    assert(*p == 89);

    // upper_bound
    nums = {1, 2, 3, 89, 99};
    p = upper_bound(nums.begin(), nums.end(), 3);
    assert(p == nums.begin() + 3);
    assert(*p == 89);

    // binary_search
    auto found = binary_search(nums.begin(), nums.end(), 2);
    assert(found);

    // equal_range
    auto seq = equal_range(nums.begin(), nums.end(), 991);
    assert(seq.first == nums.end());
    assert(seq.second == nums.end());

    seq = equal_range(nums.begin(), nums.end(), 99);
    assert(seq.first == nums.end() - 1);
    assert(seq.second == nums.end());
    assert(*seq.first == 99);
}
