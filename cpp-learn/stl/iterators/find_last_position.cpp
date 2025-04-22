/*
 The C++ Programming Language, 4th Edition
 */

#include <vector>
#include <algorithm>
#include <cassert>

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
}

template<typename T>
std::vector<T> expected(std::initializer_list<T> l) {
    return std::vector<T>{l};
}
