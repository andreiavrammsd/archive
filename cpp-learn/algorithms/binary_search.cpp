#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

int search(const std::vector<int> &numbers, const int &n) {
    size_t l = 0;
    size_t r = numbers.size();

    while (l <= r) {
        size_t mid = (l + r) / 2;

        if (n == numbers[mid]) {
            return mid;
        }

        if (n < numbers[mid]) {
            r = mid - 1;
        } else if (n > numbers[mid]) {
            l = mid + 1;
        }
    }

    return -1;
}

int main() {
    std::vector<int> numbers = {0, 1, -1, 6, 7, 8, 3};
    std::sort(numbers.begin(), numbers.end());

    for (const auto &n : numbers) {
        std::cout << n << ", ";
    }
    std::cout << std::endl;

    assert(search(numbers, -1) == 0);
    assert(search(numbers, 0) == 1);
    assert(search(numbers, 1) == 2);
    assert(search(numbers, 3) == 3);
    assert(search(numbers, 6) == 4);
    assert(search(numbers, 7) == 5);
    assert(search(numbers, 8) == 6);
    assert(search(numbers, 14) == -1);
}
