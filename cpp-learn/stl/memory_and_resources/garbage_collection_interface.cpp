/*
 The C++ Programming Language, 4th Edition
 */

#include <memory>
#include <iostream>

/**
 * The Garbage Collection Interface
 *
 * I see garbage collection as a convenient last resort after the usual techniques for preventing leaks
 * have been exhausted:
 *
 * [1] Use resource handles with the proper semantics for an application whenever possible.
 * The standard library provides string, vector,unordered_map, thread, lock_guard, and more.
 * Move semantics allow such objects to be efficiently returned from a function.
 *
 * [2] Use unique_ptrs to hold on to objects that do not implicitly manage their own resources
 * (such as pointers), need to be protected from premature deletion (because they don't have
 * proper destructors), or need to be allocated in ways that require special attention (deleters).
 *
 * [3] Use shared_ptrs to hold objects that require shared ownership.
 *
 * A garbage collector is not even a required part of a standard C++ implementation.
 */

int main() {
    /**
     * The standard library allows a programmer to specify where there are no pointers to be found
     * (e.g., in an image) and what memory should not be reclaimed even if the collector can't find a
     * pointer into it (§iso.20.6.4):
     */

    int *p = new int(1);
    std::declare_reachable(p); // the object pointed to by p must not be collected
    std::undeclare_reachable(p); // undo a declare_reachable()

    char *pp{};
    size_t sz = 1000;
    std::declare_no_pointers(pp, sz); // pp[0:n) holds no pointers
    std::undeclare_no_pointers(pp, sz); // undo a declare_no_pointers()

    /**
     * C++ garbage collectors have traditionally been conservative collectors; that is, they do not move
     * objects around in memory and have to assume that every word in memory might contain a pointer.
     * Conservative garbage collection is more efficient than it is reputed to be, especially when a program
     * doesn't generate much garbage, but declare_no_pointers() can make it very efficient by safely
     * eliminating large parts of memory from consideration. For example, we might use declare_no_pointers()
     * to tell the collector where our photographic images are in an application, so as to allow the collector
     * to ignore potentially gigabytes of non-pointer data.
     *
     * A programmer can inquire which rules for pointer safety and reclamation are in force:
     */

    std::cout << (std::get_pointer_safety() == std::pointer_safety::relaxed) << '\n';
    std::cout << (std::get_pointer_safety() == std::pointer_safety::preferred) << '\n';
    std::cout << (std::get_pointer_safety() == std::pointer_safety::strict) << '\n';

    /**
     * relaxed: Safely-derived and not safely-derived pointers are treated equivalently (as in C and
     * C++98). Collect every object that does not have a safely derived or traceable pointer to it.
     *
     * preferred: Like relaxed, but a garbage collector may be running as a leak detector and/or a
     * detector of dereferences of "bad pointers."
     *
     * strict: Safely-derived and not safely-derived pointers may be treated differently; that is, a
     * garbage collector may be running and will ignore pointers that's not safely derived.
     */
}
