/*
 The C++ Programming Language, 4th Edition
 */

#include <cinttypes>  // Aliases for common integer types
#include <cstdbool>   // C bool
#include <ccomplex>   // <complex>
#include <cfenv>      // Floating-point environment
#include <cstdalign>  // C alignment
#include <ctgmath>    // C "type generic math": <complex> and <cmath>

#include <cassert>

int main()
{
    // Aliases for common integer types
    uint8_t a = 1;
    ++a;
    assert(a == uint8_t(2));

    // C bool
    _Bool is = false;
    assert(!is);

    // <complex>
    std::complex<int> c{1};
    assert(c.real() == 1);

    // Floating-point environment

    // C alignment

    // C "type generic math": <complex> and <cmath>
}
