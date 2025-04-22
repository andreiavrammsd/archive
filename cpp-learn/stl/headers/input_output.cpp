/*
 The C++ Programming Language, 4th Edition
 */

#include <iosfwd> // Forward declarations of I/O facilities
#include <iostream> // Standard iostream objects and operations
#include <ios> // iostream bases
#include <streambuf> // Stream buffers
#include <istream> // Input stream template
#include <ostream> // Output stream template
#include <iomanip> // Manipulators
#include <sstream> // Streams to/from strings
#include <cctype> // Character classification functions
#include <fstream> // Streams to/from files
#include <cstdio> // printf() family of I/O
#include <cwchar> // printf()-style I/O of wide characters

#include <cassert>
#include <algorithm>

int main() {
    // Forward declarations of I/O facilities

    // Standard iostream objects and operations

    // iostream bases

    // Stream buffers
    std::istringstream input{"ABC"};
    std::ostringstream output;

    copy(std::istreambuf_iterator(input), std::istreambuf_iterator<char>{},
         std::ostreambuf_iterator{output});
    assert(output.str() == "ABC");

    // Input stream template

    // Output stream template

    // Manipulators

    // Streams to/from strings
    std::istringstream is{"abc"};
    assert(is.str() == "abc");

    std::ostringstream os;
    os << is.str();
    assert(os.str() == "abc");

    // Character classification functions
    assert(islower('c'));
    std::string str = "aBc";
    assert(count_if(str.begin(), str.end(), islower) == 2);

    // Streams to/from files

    // printf() family of I/O

    // printf()-style I/O of wide characters
}
