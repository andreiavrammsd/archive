/*
 The C++ Programming Language, 4th Edition
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

/**
 * Allocators
 *
 * The STL containers (§31.4) and string (Chapter 36) are resource handles that acquire and release
 * memory to hold their elements. To do so, they use allocators. The basic purpose of an allocator is
 * to provide a source of memory for a given type and a place to return that memory to once it is no
 * longer needed.
 */

template <typename T>
struct Simple_alloc {  // use new[] and delete[] to allocate and deallocate bytes
    using value_type = T;

    Simple_alloc() = default;

    T *allocate(size_t n) { return reinterpret_cast<T *>(new char[n * sizeof(T)]); }

    void deallocate(T *p, size_t) { delete[] reinterpret_cast<char *>(p); }
};

class Arena {
    void *p;
    int s;

   public:
    Arena(void *pp, int ss) : p(pp), s(ss){};  // allocate from p[0..ss-1]
    void use()
    {
        assert(p != nullptr);
        assert(s > 0);
    }
};

template <typename T>
struct My_alloc {  // use an Arena to allocate and deallocate bytes
    using value_type = T;

    Arena &a;

    My_alloc(Arena &aa) : a(aa) {}

    T *allocate(size_t) {}

    void deallocate(T *, size_t) {}
};

template <typename T>
using Arena_vec = std::vector<T, My_alloc<T>>;

template <typename T>
using Simple_vec = std::vector<T, Simple_alloc<T>>;

int main()
{
    /**
     * Basic allocator functions
     */

    const size_t n = 3;
    std::allocator<int> a;

    // allocate
    auto p = a.allocate(n);

    for (size_t i = 0; i < n; i++) {
        p[i] = i + 1;
    }
    for (size_t i = 0; i < n; i++) {
        std::cout << p[i] << ',';
    }
    std::cout << '\n';

    // deallocate
    a.deallocate(p, n);

    for (size_t i = 0; i < n; i++) {
        std::cout << p[i] << ',';
    }
    std::cout << '\n';

    /**
     * Basic allocator implementation
     *
     * Simple_alloc happens to be the simplest standards-conforming allocator.
     * Note the casts to and from char*: allocate() does not invoke constructors and
     * deallocate() does not invoke destructors; they deal in memory, not typed objects.
     */

    std::vector<int, Simple_alloc<int>> vec{10, Simple_alloc<int>{}};

    std::iota(vec.begin(), vec.end(), 0);
    std::for_each(vec.begin(), vec.end(), [](int i) { std::cout << i << ','; });

    /**
     * Custom allocator
     */

    constexpr int sz{100000};
    Arena my_arena1{new char[sz], sz};
    Arena my_arena2{new char[10 * sz], 10 * sz};

    std::vector<int> v0;  // allocate using default allocator

    std::vector<int, My_alloc<int>> v1{My_alloc<int>{my_arena1}};  // construct in my_arena1

    std::vector<int, My_alloc<int>> v2{My_alloc<int>{my_arena2}};  // construct in my_arena1

    std::vector<int, Simple_alloc<int>> v3;  // construct on free store

    // [...] the verbosity would be alleviated by the use of aliases.
    // My_alloc<std::complex<double>> Alloc2{my_arena2}; // named allocator object
    // Arena_vec<std::complex<double>> vcd{{{1, 2}, {3, 4}}, Alloc2};
    // Simple_vec<std::string> vs{"Sam Vimes", "Fred Colon", "Nobby Nobbs"};

    // An allocator imposes space overhead in a container only if its objects actually have state (like
    // My_alloc). This is usually achieved by relying on the empty-base optimization (§28.5).

    /**
     * The Default Allocator
     *
     * A default allocator that allocates using new and deallocates using delete
     * is used (by default) by all standard-library containers.
     */

    /**
     * Allocator Traits
     *
     * The allocators are "wired together" using allocator_traits. A property of an allocator, say, its
     * pointer type, is found in its trait: allocator_traits<X>::pointer. As usual, the traits technique is used
     * so that I can build an allocator for a type that does not have member types matching the requirements
     * of an allocator, such as int, and a type designed without any thought of allocators.
     */

    using allocator = std::allocator<int>;
    std::allocator_traits<allocator>::pointer ptr;
    std::allocator_traits<allocator>::value_type val;

    assert(typeid(ptr) == typeid(int *));
    assert(typeid(val) == typeid(int));

    /**
     * Pointer Traits
     *
     * An allocator uses pointer_traits to determine properties of pointers and proxy types for pointers
     */

    /**
     * Scoped Allocators
     *
     * [...] if you use Your_allocator for Your_string to allocate its elements and I use My_allocator
     * to allocate elements of My_vector then which allocator should be used for string elements
     * in My_vector<Your_allocator>>?
     *
     * [...] provides the mechanism to keep track of an outer allocator (to be used for elements)
     * and an inner allocator (to be passed to elements for their use).
     */
}
