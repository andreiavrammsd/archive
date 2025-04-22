/*
 The C++ Programming Language, 4th Edition
 */

#include <limits>
#include <iostream>
#include <cassert>
#include <climits>
#include <cfloat>

struct Quad {
};

int main() {
    char classification[std::numeric_limits<unsigned char>::max()];
    static_assert(sizeof(classification) == std::numeric_limits<unsigned char>::max());

    if (std::numeric_limits<unsigned char>::digits == std::numeric_limits<char>::digits) {
        std::cout << "chars are unsigned\n";
    }

    int i = std::numeric_limits<short>::max() + 1;
    if (i < std::numeric_limits<short>::min() || std::numeric_limits<short>::max() < i) {
        std::cout << "i cannot be stored in a short without loss of digits\n";
    }

    double d = 0.001;
    if (0 < d && d < std::numeric_limits<double>::epsilon()) {
        d = 0;
    }
    assert(d == 0.001);

    static_assert(std::numeric_limits<int>::is_specialized);
    static_assert(!std::numeric_limits<Quad>::is_specialized);

    /**
     * Limit Macros
     */

    // Integer Limit Macros: <climits>
    std::cout << CHAR_BIT << '\n';
    // CHAR_MIN
    // CHAR_MAX
    // INT_MIN
    // LONG_MAX

    // Floating-Point Limit Macros: <cfloat>
    std::cout << FLT_MIN << '\n';
    // FLT_MAX
    // FLT_DIG
    // FLT_MAX_10_EXP
    // DBL_MIN
    // DBL_MAX
    // DBL_EPSILON
}
