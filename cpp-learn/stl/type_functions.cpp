/*
 The C++ Programming Language, 4th Edition
 */

#include <iostream>
#include <limits>

int main() {
    constexpr float min = std::numeric_limits<float>::min(); // smallest positive float
    std::cout << min << std::endl;

    constexpr int szi = sizeof(int); // the number of bytes in an int
    std::cout << szi << std::endl;
}
