/*
 The C++ Programming Language, 4th Edition
 */

#include <iostream>
#include <cstring>
#include <cassert>
#include <algorithm>

const size_t sz = 100;

int length(const char *);

void odd(int *p, size_t size);

void odd2(int a[], size_t size);

void odd3(int buff[sz], size_t size);

int main() {
    char v[] = "Annemarie";
    size_t i = length(v);
    size_t j = length("Nicholas");

    assert(i == std::strlen(v));
    assert(j == std::strlen("Nicholas"));

    int a[sz];
    std::generate(std::begin(a), std::end(a), [] {
        static int i = 0;
        return ++i;
    });

    odd(a, sz);
    odd2(a, sz);
    odd3(a, sz);
}


int length(const char *s) {
    int l = 0;
    while (*s != '\0') {
        ++l;
        s++;
    }
    return l;
}

void odd(int *p, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (p[i] % 2 != 0) {
            std::cout << p[i] << std::endl;
        }
    }
}

void odd2(int a[], size_t size) {
    return odd(a, size);
}

void odd3(int buff[1020], size_t size) {
    return odd(buff, size);
}

