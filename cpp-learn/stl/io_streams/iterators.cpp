/*
 The C++ Programming Language, 4th Edition
 */

#include <iterator>
#include <sstream>
#include <cassert>

int main() {
    // In <iterator>, the standard library provides iterators to allow input and output streams to be viewed
    // as sequences [input-begin:end-of-input) and [output-begin:end-of-output)

    std::istringstream in{"1\n2.3\n3.4\n"};
    std::ostringstream out;

    copy(std::istream_iterator<double>{in}, std::istream_iterator<double, char>{},
         std::ostream_iterator<double>{out, ";"});

    assert(out.str() == "1;2.3;3.4;");

    // Stream Iterator Operations

    // istream_iterator p {st};
    // istream_iterator p {p2};
    // ostream_iterator p {st};
    // ostream_iterator p {p2};
    // ostream_iterator p {st,s};
    // p=p2
    // p2=++p
    // p2=p++
    // *p=x
    // *p++=x

    // Except for the constructors, these operations are typically used by general algorithms, such as
    // copy(), rather than directly.
}
