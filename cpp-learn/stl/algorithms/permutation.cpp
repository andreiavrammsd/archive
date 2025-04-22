/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <algorithm>

#include <cassert>
#include <iostream>
#include <vector>

/**
 * is_permutation
 * next_permutation
 * prev_permutation
 */

template<typename T>
std::vector<T> expected(std::initializer_list<T>);

int main() {
    std::vector<char> chars = {'a', 'b', 'c'};

    // is_permutation (C++11)
    auto chars2 = {'a', 'b', 'x'};
    assert(!std::is_permutation(chars.begin(), chars.end(), chars2.begin()));

    // next_permutation
    chars = {'a', 'b', 'c'};
    std::next_permutation(chars.begin(), chars.end());
    assert(chars == expected({'a', 'c', 'b'}));

    while (std::next_permutation(chars.begin(), chars.end()))
        std::cout << chars[0] << chars[1] << chars[2] << ' ';
    std::cout << '\n';

    // prev_permutation
    std::prev_permutation(chars.begin(), chars.end());
    assert(chars == expected({'c', 'b', 'a'}));
}

template<typename T>
std::vector<T> expected(std::initializer_list<T> l) {
    return std::vector<T>{l};
}

