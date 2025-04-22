/*
 The C++ Programming Language, 4th Edition
 */

#include <ratio>

/**
 * In <ratio>, we find class ratio, which provides compile-time rational arithmetic. The standard
 * library uses ratio to provide a compile-time representation of time duration and time points (§35.2)
 */

int main()
{
    using r1 = std::ratio<1, 2>;
    static_assert(r1::num == 1, "");
    static_assert(r1::den == 2, "");

    // z=ratio_add<x,y>
    // z=ratio_subtract<x,y>
    // z=ratio_multiply<x,y>
    // z=ratio_divide<x,y>
    // ratio_equal<x,y>
    // ratio_not_equal<x,y>
    // ratio_less<x,y>
    // ratio_less_equal<x,y>
    // ratio_not_equal<x,y>
    // ratio_greater<x,y>
    // ratio_greater_equal<x,y>

    static_assert(std::ratio_add<std::ratio<1, 3>, std::ratio<1, 6>>::num == 1, "problem: 1/3+1/6 != 1/2");
    static_assert(std::ratio_add<std::ratio<1, 3>, std::ratio<1, 6>>::den == 2, "problem: 1/3+1/6 != 1/2");

    // Obviously, this is not a convenient way of expressing numbers and arithmetic. In <chrono>, we find
    // the conventional notation (e.g., + and *) for rational arithmetic for time (§35.2). Similarly, to help
    // express unit values_, the standard library provides common SI magnitude names:

    using milli = std::ratio<1, 1000>;
    static_assert(milli::num == 1);
    static_assert(milli::den == 1000);

    static_assert(std::milli::num == 1);
    static_assert(std::milli::den == 1000);
}
