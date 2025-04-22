/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <algorithm>

#include <vector>
#include <string>
#include <cassert>

/**
 * is_heap
 * is_heap_until
 * make_heap
 * push_heap
 * pop_heap
 * sort_heap
 */

template<typename T>
std::vector<T> expected(std::initializer_list<T>);

int main() {
    // A heap is a compact data structure that keeps the element with the highest value first. Think of a
    // heap as a representation of a binary tree. The heap algorithms allow a programmer to treat a
    // random-access sequence as a heap:
    std::vector<int> nums = {1, 5, 2, 3};

    // is_heap (C++11)
    assert(!is_heap(nums.begin(), nums.end()));;

    // is_heap_until (C++11)
    make_heap(nums.begin(), nums.end());
    pop_heap(nums.begin(), nums.end());
    auto p = is_heap_until(nums.begin(), nums.end());
    assert(p == nums.begin() + 3);
    assert(nums == expected({3, 1, 2, 5}));

    // make_heap
    make_heap(nums.begin(), nums.end());
    assert(nums == expected({5, 3, 2, 1}));
    assert(is_heap(nums.begin(), nums.end()));

    // push_heap
    push_heap(nums.begin(), nums.end());
    assert(is_heap(nums.begin(), nums.end()));

    // pop_heap
    pop_heap(nums.begin(), nums.end());
    assert(!is_heap(nums.begin(), nums.end()));
    assert(is_heap(nums.begin(), nums.end() - 1));

    // sort_heap
    sort_heap(nums.begin(), nums.end());
    assert(!is_heap(nums.begin(), nums.end()));

    // Think of the end, e, of a heap [b:e) as a pointer, which it decremented by pop_heap() and incremented
    // by push_heap(). The largest element is extracted by reading through b (e.g., x=*b ) and then
    // doing a pop_heap(). A new element is inserted by writing through e (e.g., *e=x ) and then doing a
    // push_heap().

    std::string heap = "abcd";

    make_heap(heap.begin(), heap.end());
    assert(heap == "dbca");

    pop_heap(heap.begin(), heap.end());
    assert(heap == "cbad");

    pop_heap(heap.begin(), heap.end() - 1);
    assert(heap == "bacd");

    pop_heap(heap.begin(), heap.end() - 2);
    assert(heap == "abcd");

    *(heap.end() - 3) = 'z';
    push_heap(heap.begin(), heap.end() - 2);
    assert(heap == "zacd");

    *(heap.end() - 2) = 'y';
    push_heap(heap.begin(), heap.end() - 1);
    assert(heap == "zayd");

    *(heap.end() - 1) = 'x';
    push_heap(heap.begin(), heap.end());
    assert(heap == "zxya");

    sort(heap.begin(), heap.end());
    assert(heap == "axyz");

    reverse(heap.begin(), heap.end());
    assert(heap == "zyxa");

    // The way to understand the changes to s is that a user reads only s[0] and writes only s[x] where x is
    // the index of the current end of the heap. The heap removes an element (always s[0]) by swapping it with s[x].
    // The point of a heap is to provide fast addition of elements and fast access to the element with
    // the highest value. The main use of heaps is to implement priority queues.
}

template<typename T>
std::vector<T> expected(std::initializer_list<T> l) {
    return std::vector<T>{l};
}
