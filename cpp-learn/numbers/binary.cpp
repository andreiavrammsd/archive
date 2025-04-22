#include <string>
#include <map>
#include <cmath>
#include <cassert>

int bin2dec(int bin) {
    int dec = 0, pos = 0;

    while (bin > 0) {
        dec += (int) std::pow(2, pos) * (bin % 10);
        pos++;
        bin /= 10;
    }

    return dec;
}

int dec2bin(int dec) {
    std::string decStr;

    while (dec) {
        decStr.insert(0, std::to_string(dec % 2));
        dec /= 2;
    }

    return decStr.empty() ? 0 : std::stoi(decStr);
}

int main() {
    std::map<int, int> numbers = {
            {0,        0},
            {1,        1},
            {1101,     13},
            {1111,     15},
            {11111111, 255},
    };

    for (const auto &number : numbers) {
        assert(bin2dec(number.first) == number.second);
    }

    for (const auto &number : numbers) {
        assert(dec2bin(number.second) == number.first);
    }
}
