/*
 The C++ Programming Language, 4th Edition
 */

#include <vector>
#include <forward_list>
#include <algorithm>
#include <iostream>
#include <cassert>

/**
 * The iterator tags are types used to select among algorithms based on the type of an iterator.
 *
 * input_iterator_tag
 * output_iterator_tag
 * forward_iterator_tag (forward_list, unordered_set, unordered_multiset, unordered_map, and unordered_multimap)
 * bidirectional_iterator_tag (list, set, multiset, map, multimap)
 * random_access_iterator_tag (vector, deque, array, built-in arrays, and string)
 */

// for random-access iterators
template <typename Ran>
void sort_helper(Ran beg, Ran end, std::random_access_iterator_tag)
{  // we can subscript into [beg:end]
    sort(beg, end);
}

// for forward iterators
template <typename For>
void sort_helper(For beg, For end, std::forward_iterator_tag)
{  // we can traverse [beg:end]
    auto value = *beg;
    std::vector<decltype(value)> v{beg, end};  // initialize a vector from [beg:end]
    std::sort(v.begin(), v.end());             // sort the vector
    copy(v.begin(), v.end(), beg);             // copy the elements back
}

template <typename C>
using Iterator_type = typename C::iterator;

// The idea is that to find a property of an iterator, you look in its iterator_traits (§28.2.4), rather than
// at the iterator itself. To avoid directly referring to the iterator_traits , which after all is just an
// implementation detail, we can define an alias.

template <typename Iter>
using Iterator_category = typename std::iterator_traits<Iter>::iterator_category;

template <typename C>
void sort(C &c)
{
    using Iter = Iterator_type<C>;
    sort_helper(c.begin(), c.end(), Iterator_category<Iter>{});
}

template <typename C>
void print(const C &c)
{
    for (const auto &elem : c) {
        std::cout << elem << ", ";
    }
    std::cout << std::endl;
}

// Distance
template <typename Iter>
using Difference_type = typename std::iterator_traits<Iter>::difference_type;

template <typename Iter>
size_t dist(Iter p, Iter q)
{
    // Difference_type<Iter> dist = std::distance(p, q);
    // vs
    auto dist = std::distance(p, q);

    return dist;
}

int main()
{
    std::vector<char> letters = {'Z', 'A', 'B'};
    sort(letters);
    print(letters);

    std::forward_list<int> digits = {5, 6, 0};
    sort(digits);
    print(digits);

    auto beg = letters.begin();
    auto end = letters.end();
    assert(dist(beg, end) == 3);

    --end;
    --end;
    --end;
    assert(dist(beg, end) == 0);
}
