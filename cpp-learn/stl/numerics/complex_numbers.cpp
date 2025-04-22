/*
 The C++ Programming Language, 4th Edition
 */

#include <complex>
#include <cassert>
#include <iostream>

int main()
{
    // The standard library provides complex number types complex<float>, complex<double>, and
    // complex<long double>. A complex<Scalar> where Scalar is some other type supporting the
    // usual arithmetic operations usually works but is not guaranteed to be portable.

    std::complex<double> c1{1, 2};
    std::complex<double> c2 = c1 + std::complex<double>{2, 3};
    std::cout << c2;
    assert(c2 == (std::complex<double>{3, 5}));

    // Operators
    // z1+z2
    // z1−z2
    // z1*z2
    // z1/z2
    // z1==z2
    // z1!=z2
    // norm(z)
    // conj(z)
    // polar(x,y)
    // real(z)
    // imag(z)
    // abs(z)
    // arg(z)
    // out<<z
    // in>>z
}
