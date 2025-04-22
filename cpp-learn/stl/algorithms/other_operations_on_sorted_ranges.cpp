/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <algorithm>

#include <vector>
#include <cassert>

/**
 * merge
 * inplace_merge
 */

template<typename T>
std::vector<T> expected(std::initializer_list<T>);

int main() {
    std::vector<int> nums = {1, 2, 3}; // sorted range

    // merge
    std::vector<int> nums2 = {3, 4, -1};
    sort(nums2.begin(), nums2.end());

    std::vector<int> out;

    merge(nums.begin(), nums.end(), nums2.begin(), nums2.end(), std::back_inserter(out));
    assert(out == expected({-1, 1, 2, 3, 3, 4}));

    // inplace_merge
    nums = {7, 5, 4, 0, 9, 8, 2};
    inplace_merge(nums.begin(), nums.begin() + 1, nums.end());
    assert(nums == expected({5, 4, 0, 7, 9, 8, 2}));
}

template<typename T>
std::vector<T> expected(std::initializer_list<T> l) {
    return std::vector<T>{l};
}
