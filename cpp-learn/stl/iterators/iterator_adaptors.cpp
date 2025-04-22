/*
 The C++ Programming Language, 4th Edition
 */

#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <forward_list>
#include <iterator>
#include <sstream>
#include <memory>

/**
 * reverse_iterator         Iterate backward
 * back_insert_iterator     Insert at end
 * front_insert_iterator    Insert at beginning
 * insert_iterator          Insert anywhere
 * move_iterator            Move rather than copy
 * raw_storage_iterator     Write to uninitialized storage
 */

template<typename T>
std::vector<T> expected(std::initializer_list<T> l);

template<typename C, typename Val>
auto find_last(C &c, Val v) -> decltype(c.begin()) { // use C's iterator in the interface
    auto ri = find(c.rbegin(), c.rend(), v);
    if (ri == c.rend()) return c.end(); // use c.end() to indicate "not found"
    return prev(ri.base());

    // For a reverse_iterator, ri.base() returns an iterator pointing one beyond the position pointed to by ri.
    // So, to get an iterator pointing to the same element as the reverse iterator ri, I have to return
    // ri.base()-1 . Howev er, my container may be a list that does not support - for its iterators, so I use
    // prev() instead.
}

template<typename Iterator>
void print(const Iterator it) {
    for (const auto i : it) {
        std::cout << i;
    }
    std::cout << '\n';
}

int main() {
    std::vector<int> v{1, 2, 2, 3};
    std::vector<int> out;

    /**
     * reverse_iterator
     */
    out.resize(v.size());
    copy(v.crbegin(), v.crend(), out.begin());
    assert(out == expected({3, 2, 2, 1}));

    assert(find(v.begin(), v.end(), 2) == v.begin() + 1);
    assert(find_last(v, 2) == v.begin() + 2);

    /**
     * back_insert_iterator
     */
    out.clear();
    copy(v.cbegin(), v.cend(), back_inserter(out));
    assert(out == expected({1, 2, 2, 3}));

    /**
     * front_insert_iterator
     */
    std::forward_list<int> list;
    copy(v.cbegin(), v.cend(), front_inserter(list));

    assert(list.front() == 3);
    list.pop_front();

    assert(list.front() == 2);
    list.pop_front();

    assert(list.front() == 2);
    list.pop_front();

    assert(list.front() == 1);
    list.pop_front();

    /**
     * insert_iterator
     */
    out.clear();
    out.resize(v.size());
    copy(v.cbegin(), v.cend(), inserter(out, out.begin() + 2));
    assert(out == expected({0, 0, 1, 2, 2, 3, 0, 0}));

    /**
     * move_iterator
     */
    std::vector<std::string> words{"the", "quick", "brown", "fox"};
    std::vector<std::string> words_out(make_move_iterator(words.begin()), make_move_iterator(words.end()));

    std::stringstream ws;

    copy(words.begin(), words.end(), std::ostream_iterator<std::string>(ws));
    assert(ws.str().empty());

    ws.str("");
    copy(words_out.begin(), words_out.end(), std::ostream_iterator<std::string>(ws));
    assert(ws.str().length() == 16);

    /**
     * raw_storage_iterator
     *
     * https://en.cppreference.com/w/cpp/memory/raw_storage_iterator
     */
    words = {"the", "quick", "brown", "fox"};
    std::string *wordsp = std::allocator<std::string>().allocate(4);

    copy(words.begin(), words.end(), std::raw_storage_iterator<std::string *, std::string>(wordsp));

    for (std::string *i = wordsp; i != wordsp + 4; ++i) {
        std::cout << *i << '\n';
        i->~basic_string<char>();
    }

    std::allocator<std::string>().deallocate(wordsp, 4);
}

template<typename T>
std::vector<T> expected(std::initializer_list<T> l) {
    return std::vector<T>{l};
}
