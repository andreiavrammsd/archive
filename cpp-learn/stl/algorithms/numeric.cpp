/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <numeric>

#include <array>
#include <vector>
#include <cassert>

/**
 * iota
 * accumulate
 * inner_product
 * adjacent_difference
 * partial_sum
 * reduce
 * exclusive_scan
 * inclusive_scan
 * transform_reduce
 * transform_exclusive_scan
 * transform_inclusive_scan
 */

int main() {
    // iota (C++11)
    std::array<char, 3> chars;
    std::iota(chars.begin(), chars.end(), 'A');
    assert(chars == (std::array<char, 3>{'A', 'B', 'C'}));

    std::vector<int> nums{1, 2, 3};

    // accumulate
    assert(accumulate(nums.begin(), nums.end(), 0) == 6);
    assert(accumulate(nums.begin(), nums.end(), 10, std::multiplies<>{}) == 60);

    // inner_product
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{4, 5, 0};
    assert(inner_product(a.begin(), a.end(), b.begin(), 0) == 14);

    // adjacent_difference
    // Given a sequence a, b, c, d, etc., adjacent_difference() produces a, b-a, c-b, d-c, etc.
    std::vector<double> temps = {17, 19, 20, 20, 17};
    adjacent_difference(temps.begin(), temps.end(), temps.begin());
    assert(temps == (std::vector<double>{17, 2, 1, 0, -3}));

    // partial_sum
    // Given a sequence a, b, c, d, etc., partial_sum() produces a, a+b, a+b+c, a+b+c+d, etc.
    partial_sum(temps.begin(), temps.end(), temps.begin());
    assert(temps == (std::vector<double>{17, 19, 20, 20, 17}));

    // reduce (C++17)
    assert(std::reduce(nums.begin(), nums.end()) == 6);
    assert(std::reduce(nums.begin(), nums.end(), 2, std::multiplies{}) == 12);

    // inclusive_scan (C++17)
    std::vector<int> nums_result;
    std::inclusive_scan(nums.cbegin(), nums.cend(), std::back_inserter(nums_result));
    assert(nums_result == (std::vector<int>{1, 3, 6}));

    // exclusive_scan (C++17)
    nums_result.clear();
    std::exclusive_scan(nums.cbegin(), nums.cend(), std::back_inserter(nums_result), 5);
    assert(nums_result == (std::vector<int>{5, 6, 8}));

    // transform_reduce (C++17)
    std::vector<int> as{1, 2, 3, 4};
    std::vector<int> bs{-1, -2, -3, -4};

    auto res = std::transform_reduce(as.cbegin(), as.cend(), bs.cbegin(), 0);
    assert(res == -30);

    res = std::transform_reduce(as.cbegin(), as.cend(), bs.cbegin(), 1, std::plus{}, std::minus{});
    assert(res == 21);

    res = std::transform_reduce(as.cbegin(), as.cend(), 1, std::plus{}, std::negate{});
    assert(res == -9);

    // transform_exclusive_scan (C++17)
    bs.clear();
    std::transform_exclusive_scan(as.cbegin(), as.cend(), std::back_inserter(bs),
                                  1, std::plus{}, [](int x) { return x + 1; });
    assert(bs == (std::vector<int>{1, 3, 6, 10}));

    // transform_inclusive_scan (C++17)
    bs.clear();
    std::transform_inclusive_scan(as.cbegin(), as.cend(), std::back_inserter(bs),
                                  std::plus{}, [](int x) { return -x * 2; });
    assert(bs == (std::vector<int>{-2, -6, -12, -20}));
}
