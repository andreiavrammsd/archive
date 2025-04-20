#include <cassert>
#include <vector>

// best: O(n) T, O(1) S
// avg: O(n^2) T, O(1) S
// worst: O(n^2) T, O(1) S
std::vector<int> insertionSort(std::vector<int> array) {
    if (array.empty()) {
        return {};
    }

    size_t end = 1;

    for (size_t i = 1; i < array.size(); ++i) {
        for (size_t j = end; j > 0; --j) {
            if (array[j] < array[j - 1]) {
                std::swap(array[j], array[j - 1]);
            }
        }
        ++end;
    }

    return array;
}

int main() { assert(insertionSort({8, 5, 2, 9, 5, 6, 3}) == (std::vector<int>{2, 3, 5, 5, 6, 8, 9})); }
