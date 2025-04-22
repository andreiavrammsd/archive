/*
 The C++ Programming Language, 4th Edition
 https://en.wikipedia.org/wiki/C%2B%2B14
 https://en.wikipedia.org/wiki/C%2B%2B17
 */

#include <string>

std::string operator "" _s(const char *p, size_t n) {
    return std::string{p, n}; // requires free-store allocation
}

unsigned long long operator "" _s(unsigned long long s) {
    return s;
}

unsigned long long operator "" _b(unsigned long long b) {
    return b;
}

long double operator "" _km(long double km) {
    return km;
}

unsigned long long operator "" _x(unsigned long long x) {
    return x;
}

int main() {
    // C++ provides literals for a variety of built-in types (§6.2.6):
    // 123; // int
    // 1.2; // double
    // 1.2F; // float
    // 'a'; // char
    // 1ULL; // unsigned long long
    // 0xD0; // hexadecimal unsigned
    // "as"; // C-style string (const char[3])

    // In addition, we can define literals for user-defined types and new forms of literals for built-in types.
    // For example:
    "Hi!"_s; // string, not "zero-terminated array of char"
    // 1.2i; // imaginary
    101010111000101_b; // binary
    123_s; // seconds
    123.56_km; // not miles! (units)
    12345678901234567_x; // extended-precision

    /**
     * C++14
     */

    // "s", for creating the various std::basic_string types.
    using namespace std::string_literals;
    "str"s;

    // "h", "min", "s", "ms", "us", "ns", for creating the corresponding std::chrono::duration time intervals.

    // "if", "i", "il", for creating the corresponding std::complex<float>, std::complex<double>
    // and std::complex<long double> imaginary numbers.

    static_assert(0b110 == 6); // Binary literals

    /**
     * C++17
     */

    // UTF-8 (u8) character literals: u8'c'
    // Hexadecimal floating-point literals: 0x1.2p0
}
