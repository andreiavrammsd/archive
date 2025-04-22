/*
 The C++ Programming Language, 4th Edition
 */

#include <string>
#include <sstream>
#include <cassert>
#include <iostream>

template<typename Target = std::string, typename Source = std::string>
Target to(Source arg) { // convert Source to Target
    std::stringstream interpreter;
    Target result;

    if (!(interpreter << arg) // write arg into stream
        || !(interpreter >> result) // read result from stream
        || !(interpreter >> std::ws).eof()) { // stuff left in stream?
        throw std::runtime_error{"to<>() failed"};
    }

    return result;
}

// This implementation of to() is a bit heavyweight for combinations of simple types, such as
// to<double>(int), but improved implementations can be supplied as specializations (§25.3).
template<>
double to(int arg) {
    return static_cast<double>(arg);
}

int main() {
    auto x1 = to<std::string, double>(1.2); // very explicit (and verbose)
    assert(x1 == "1.2");

    auto x2 = to<std::string>(1.2); // Source is deduced to double
    assert(x2 == "1.2");

    auto x3 = to<>(1.2); // Target is defaulted to string; Source is deduced to double
    assert(x3 == "1.2");

    auto x4 = to(1.2); // the <> is redundant
    assert(x4 == "1.2");

    auto x5 = to<double>(1);
    assert(x5 == 1);
}
