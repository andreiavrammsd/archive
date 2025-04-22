/*
 The C++ Programming Language, 4th Edition
 */

#include <new>
#include <iostream>

int main() {
    // Throws exception if cannot allocate memory
    int *p = new int[3];
    delete[] p;

    // Does not throw exception if cannot allocate memory
    p = new(std::nothrow) int[3];
    if (p == nullptr) {
        std::cout << "cannot allocate memory" << std::endl;
        return 1;
    }
    operator delete[](p, std::nothrow);
}
