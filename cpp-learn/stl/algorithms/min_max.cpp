/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <algorithm>

#include <vector>
#include <cassert>

/**
 * max
 * max_element
 * min
 * min_element
 * minmax
 * minmax_element
 * clamp
 */

int main() {
    std::vector<int> nums = {1, -1, 2};

    // max
    assert(std::max(1, 2) == 2);
    assert(std::max({1, 2, 3}) == 3);

    // max_element
    auto pmax = max_element(nums.cbegin(), nums.cend());
    assert(pmax == nums.cbegin() + 2);
    assert(*pmax == 2);

    // min
    assert(std::min(1, 2) == 1);
    assert(std::min({1, 2, 4}, [](int x, int y) { return x - y == 1; }) == 2);

    // min_element
    auto p = min_element(nums.begin(), nums.end());
    assert(p == nums.begin() + 1);

    // minmax (C++11)
    assert(std::minmax({1, -1, 5, 2, 3, 4}) == (std::pair<int, int>{-1, 5}));

    // minmax_element (C++11)
    auto pmm = minmax_element(nums.begin(), nums.end());
    assert(pmm.first == nums.begin() + 1);
    assert(*pmm.first == -1);
    assert(pmm.second == nums.begin() + 2);
    assert(*pmm.second == 2);

    // ++std::min(0, 1); // the result of min(x,y) is a const int&
    // ++std::min({1, 2}); // error: the result of min({x,y}) is an rvalue (an initializer_list is immutable)

    // clamp (C++17)
    assert(std::clamp(0.23, -1.0, 1.0) == 0.23);
    assert(std::clamp(0.23, -1.0, .1) == .1);
}
