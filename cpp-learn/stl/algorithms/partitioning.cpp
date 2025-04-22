/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <algorithm>

#include <vector>
#include <cassert>

/**
 * is_partitioned
 * partition
 * partition_copy
 * stable_partition
 * partition_point
 */

template<typename T>
std::vector<T> expected(std::initializer_list<T>);

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5, 7, 9};
    std::vector<int> out;

    // is_partitioned (C++11)
    assert(!is_partitioned(nums.begin(), nums.end(), [](int x) { return x % 2 == 0; }));

    nums = {1, 2, 3, 4, 5, 7, 9};
    assert(is_partitioned(nums.begin() + 3, nums.end(), [](int x) { return x % 2 == 0; }));

    // partition
    nums = {1, 2, 3, 4, 5};
    auto p = partition(nums.begin(), nums.end(), [](int x) { return x % 2 == 0; });

    out.clear();
    copy(nums.begin(), p, std::back_inserter(out));
    assert(out == expected({4, 2}));

    out.clear();
    copy(p, nums.end(), std::back_inserter(out));
    assert(out == expected({3, 1, 5}));

    // partition_copy (C++11)
    std::vector<char> chars = {'a', 'b', 'c', 'd', 'e'};
    std::vector<char> out1;
    std::vector<char> out2;

    auto vowel = [](char c) {
        const static auto vowels = {'a', 'e', 'i', 'o', 'u'};
        return std::find(vowels.begin(), vowels.end(), c) != vowels.end();
    };

    partition_copy(chars.begin(), chars.end(),
                   std::back_inserter(out1), std::back_inserter(out2),
                   vowel);
    assert(all_of(out1.begin(), out1.end(), vowel));
    assert(none_of(out2.begin(), out2.end(), vowel));

    // stable_partition
    nums = {1, 2, 3, 4, 5};
    p = stable_partition(nums.begin(), nums.end(), [](int x) { return x % 2 == 0; });

    out.clear();
    copy(nums.begin(), p, std::back_inserter(out));
    assert(out == expected({2, 4}));

    out.clear();
    copy(p, nums.end(), std::back_inserter(out));
    assert(out == expected({1, 3, 5}));

    // partition_point (C++11)
    auto pred = [](int x) {
        return x % 2 != 0;
    };
    std::partition(nums.begin(), nums.end(), pred);
    p = partition_point(nums.begin(), nums.end(), pred);

    assert(all_of(nums.begin(), p, pred));
    assert(none_of(p, nums.end(), pred));
}

template<typename T>
std::vector<T> expected(std::initializer_list<T> l) {
    return std::vector<T>{l};
}
