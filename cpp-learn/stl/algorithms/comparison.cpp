/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <algorithm>

#include <vector>
#include <string>
#include <cassert>

/**
 * equal
 * lexicographical_compare
 * lexicographical_compare_three_way
 */

int main() {
    // equal
    std::vector<int> nums = {1, 2, 3, 3};
    std::vector<int> nums2 = {1, 2, 3, 3, 4};

    auto eq = equal(nums.begin(), nums.end(), nums2.begin());
    assert(eq);

    eq = equal(nums.begin(), nums.end(), nums2.begin(), [](int x, int y) { return x == y; });
    assert(eq);

    // lexicographical_compare (C++11)
    std::string n1{"10000"};
    std::string n2{"999"};
    assert(std::lexicographical_compare(n1.begin(), n1.end(), n2.begin(), n2.end()));

    n1 = "Zebra";
    n2 = "Aardvark";
    assert(!std::lexicographical_compare(n1.begin(), n1.end(), n2.begin(), n2.end()));
}
