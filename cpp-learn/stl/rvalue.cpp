#include <cassert>

/*
 The C++ Programming Language, 4th Edition
 */

template <class T>
void swap_by_copy(T &a, T &b)
{              // "old-style swap"
    T tmp{a};  // now we have two copies of a
    a = b;     // now we have two copies of b
    b = tmp;   // now we have two copies of tmp (aka a)
}

template <class T>
void swap_by_move(T &a, T &b)
{                                 // "perfect swap" (almost)
    T tmp{static_cast<T &&>(a)};  // the initialization may write to a
    a = static_cast<T &&>(b);     // the assignment may write to b
    b = static_cast<T &&>(tmp);   // the assignment may write to tmp
}  // The result value of static_cast<T&&>(x) is an rvalue of type T&& for x.

int main()
{
    int x = 1;
    int y = 2;

    swap_by_copy(x, y);
    assert(x == 2 && y == 1);

    swap_by_move(x, y);
    assert(x == 1 && y == 2);
}
