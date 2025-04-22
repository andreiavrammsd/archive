/*
 Since C++17
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 */

#include <vector>
#include <unordered_map>
#include <cassert>
#include <iostream>

using Coordinate = std::pair<int, int>;

Coordinate origin() {
    return Coordinate{1, 2};
}

std::pair<int, bool> two_values() {
    return std::pair{1, true};
}

int main() {
    const auto[x, y] = origin();
    assert(x == 1);
    assert(y == 2);

    std::vector<Coordinate> coords = {
            {1, 2},
            {0, 0},
    };
    for (const auto&[x, y] : coords) {
        std::cout << "x = " << x << ", y = " << y << '\n';
    }

    std::unordered_map<std::string, int> mapping{
            {"a", 1},
            {"b", 2},
            {"c", 3},
    };
    for (const auto &[key, value] : mapping) {
        std::cout << key << ": " << value << '\n';
    }

    if (auto[number, ok] = two_values(); ok) {
        std::cout << number << '\n';
    }
}
