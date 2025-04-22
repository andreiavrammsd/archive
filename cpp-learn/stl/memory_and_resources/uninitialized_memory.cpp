/*
 The C++ Programming Language, 4th Edition
 */

#include <memory>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

/**
 * Uninitialized Memory
 *
 * Most of the time, it is best to avoid uninitialized memory. Doing so simplifies programming and
 * eliminates many kinds of errors. However, in relatively rare cases, such as when writing memory
 * allocators, implementing containers, and dealing directly with hardware, direct use of uninitialized
 * memory, also known as raw memory, is essential.
 */

int main()
{
    /**
     * Temporary Buffers
     *
     * Algorithms often require temporary space to perform acceptably. Often, such temporary space is
     * best allocated in one operation but not initialized until a particular location is actually needed.
     */

    // get_temporary_buffer - tries to allocate space for n or more objects of type X
    //
    // If it succeeds [...], it returns a pointer to the first uninitialized space and the
    // number of objects of type X that will fit into that space;
    // otherwise, the second value of the pair is zero.
    //
    // The idea is that a system may keep space ready for fast allocation so that requesting space for
    // n objects of a given size may yield space for more than n. It may also yield less, however, so one
    // way of using get_temporary_buffer() is to optimistically ask for a lot and then use what happens to
    // be available.
    ptrdiff_t len = 10;
    std::pair<int *, ptrdiff_t> buff = std::get_temporary_buffer<int>(len);

    assert(buff.second <= len);

    buff.first[0] = 1;
    assert(buff.first[0] == 1);

    // return_temporary_buffer
    std::return_temporary_buffer(buff.first);
    assert(buff.first[0] != 1);

    /**
     * raw_storage_iterator
     *
     * The standard algorithms that write into a sequence assume that the elements of that sequence have
     * been previously initialized. That is, the algorithms use assignment rather than copy construction
     * for writing. Consequently, we cannot use uninitialized memory as the immediate target of an
     * algorithm. This can be unfortunate because assignment can be significantly more expensive than
     * initialization, and to initialize immediately before overwriting is a waste. The solution is to use a
     * raw_storage_iterator from <memory> that initializes instead of assigns:
     *
     * Note that there are no == or != operators for raw_storage_iterator, so don't try to use it to write to
     * a [b:e) range. For example iota(b,e,0) (§40.6) will not work if b and e are raw_storage_iterators.
     *
     * Don’t mess with uninitialized memory unless you absolutely have to.
     */

    auto pp = std::get_temporary_buffer<std::string>(1000);
    if (pp.second < 1000) {
        // ... handle allocation failure ...
    }

    auto seed = new (std::string[4]);
    seed[0] = "A";
    seed[1] = "B";
    seed[2] = "C";
    seed[3] = "D";

    auto p = std::raw_storage_iterator<std::string *, std::string>(pp.first);  // the iterator

    size_t sz = 10;

    std::generate_n(p, sz, [&] {
        std::next_permutation(seed, seed + 3);
        return *seed;
    });

    for (size_t i = 0; i < sz; ++i) {
        std::cout << *p.base() << '\n';
        ++p;
    }

    std::return_temporary_buffer(pp.first);
}
