#include <map>
#include <cassert>
#include <iostream>

int fibonacci(int n) {
    if (n == 0) {
        return 0;
    }

    if (n == 1 || n == 2) {
        return 1;
    }

    int a = 1, b = 1, f = 0;

    for (auto i = 2; i < n; i++) {
        f = a + b;
        a = b;
        b = f;
    }

    return f;
}

int main() {
    std::map<int, int> nums = {
            {0,  0},
            {1,  1},
            {2,  1},
            {3,  2},
            {5,  5},
            {6,  8},
            {10, 55},
            {15, 610},
    };

    for (const auto &n : nums) {
        std::cout << fibonacci(n.first) << ": " << n.second << std::endl;
        assert(fibonacci(n.first) == n.second);
    }
}
