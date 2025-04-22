/*
 The C++ Programming Language, 4th Edition
 */

#include <bitset>
#include <cassert>
#include <iostream>
#include <cmath>

int main() {
    std::bitset<6> bs{49};

    assert(bs.size() == 6);
    for (size_t i = 0; i < bs.size(); ++i) {
        std::cout << bs[i];
    }
    std::cout << '\n';

    bs.set(0, false);
    bs.set(1, true);
    std::cout << bs.to_string() << '\n';
    assert(!bs.all());
    assert(bs.any());
    assert(!bs.none());
    assert(bs.count() == 3);

    bs.reset();
    assert(!bs.all());
    assert(!bs.any());
    assert(bs.none());
    assert(bs.count() == 0);

    assert(!bs[0]);
    bs.flip(0);
    assert(bs[0]);

    bs.set();
    assert(bs.all());
    std::cout << bs.to_string() << '\n';

    // compare
    assert(std::bitset<6>{255} == bs);

    std::bitset<8> num("00110001");
    assert(num == std::bitset<8>(49));
}
