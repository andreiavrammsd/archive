/*
 The C++ Programming Language, 4th Edition
 */

#include <cerrno>
#include <cmath>
#include <cassert>

int main() {
    // In <cmath> we find what are commonly called the standard mathematical functions:

    // abs(x)
    // ceil(x)
    // floor(x)
    // sqrt(x)
    // cos(x)
    // sin(x)
    // tan(x)
    // acos(x)
    // asin(x)
    // atan(x)
    // sinh(x)
    // cosh(x)
    // tanh(x)
    // exp(x)
    // log(x)
    // log10(x)

    // Errors are reported by setting errno from <cerrno> to EDOM for a domain error and to ERANGE
    // for a range error.
    errno = 0;

    sqrt(-1);
    assert(errno == EDOM);

    pow(std::numeric_limits<double>::max(), 2);
    assert(errno == ERANGE);

    // For historical reasons, a few mathematical functions are found in <cstdlib>
    // n2=abs(n)
    // n2=labs(n)
    // n2=llabs(n)
    // p=div(n,d)
    // p=ldiv(n,d)
    // p=lldiv(n,d)

    // There is a separate ISO standard for special mathematical functions [C++Math,2010]. An
    // implementation may add these functions to <cmath>:
    // assoc_laguerre()
    // comp_ellint_2()
    // cyl_bessel_k()
    // ellint_3()
    // legendre()
    // sph_neumann()
    // assoc_legendre()
    // comp_ellint_3()
    // cyl_neumann()
    // expint()
    // riemann_zeta()
    // beta()
    // cyl_bessel_i()
    // ellint_1()
    // hermite()
    // sph_bessel()
    // comp_ellint_1()
    // cyl_bessel_j()
    // ellint_2()
    // laguerre()
    // sph_legendre()
}
