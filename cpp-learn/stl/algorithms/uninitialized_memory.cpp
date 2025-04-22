/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <memory>

#include <vector>
#include <cassert>

/**
 * uninitialized_copy
 * uninitialized_copy_n
 * uninitialized_fill
 * uninitialized_fill_n
 * uninitialized_move
 * uninitialized_move_n
 * uninitialized_default_construct
 * uninitialized_default_construct_n
 * uninitialized_value_construct
 * uninitialized_value_construct_n
 * destroy_at
 * destroy
 * destroy_n
 */

template <typename T>
std::vector<T> expected(std::initializer_list<T>);

int main()
{
    std::vector<int> nums(3);
    std::vector<int> out(nums.size());

    // uninitialized_fill
    uninitialized_fill(nums.begin(), nums.end(), 7);
    assert(nums == expected({7, 7, 7}));

    // uninitialized_fill_n

    // uninitialized_move_n (C++17)

    // uninitialized_move_n (C++17)

    // uninitialized_copy
    uninitialized_copy(nums.begin(), nums.end(), out.begin());
    assert(out == expected({7, 7, 7}));

    // Uninitialized sequences should only occur at the lowest level of programming, usually inside
    // the implementation of containers. Elements that are targets of uninitialized_fill() or
    // uninitialized_copy() must be of built-in type or uninitialized.
    //
    // vector<string> vs {"Breugel","El Greco","Delacroix","Constable"};
    // vector<string> vs2 {"Hals","Goya","Renoir","Turner"};
    // copy(vs.begin(),vs.end(),vs2.begin()); // OK
    // uninitialized_copy(vs.begin(),vs.end(),vs2.begin()); // leaks!

    // uninitialized_copy_n (C++11)
    out.clear();
    out.resize(2);
    uninitialized_copy_n(nums.begin(), 1, out.begin());
    assert(out == expected({7, 0}));

    // uninitialized_default_construct (C++17)

    // uninitialized_default_construct_n (C++17)

    // uninitialized_value_construct (C++17)

    // uninitialized_value_construct_n (C++17)

    // destroy_at (C++17)
    struct Destroy {
        int i;

        ~Destroy() { i = 0; }
    };

    Destroy destroy{1};
    assert(destroy.i == 1);
    std::destroy_at(std::addressof(destroy));
    assert(destroy.i == 0);

    // destroy (C++17)

    // destroy_n (C++17)
}

template <typename T>
std::vector<T> expected(std::initializer_list<T> l)
{
    return std::vector<T>{l};
}
