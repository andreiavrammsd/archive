#include <cassert>
#include <vector>

// best: O(n^2) T, O(1) S
// avg: O(n^2) T, O(1) S
// worst: O(n^2) T, O(1) S
std::vector<int> selectionSort(std::vector<int> array) {
    if (array.empty()) {
        return {};
    }

    size_t begin_index = 0;
    size_t smallest_index = 0;

    while (begin_index < array.size() - 1) {
        for (size_t i = begin_index + 1; i < array.size(); ++i) {
            if (array[i] < array[smallest_index]) {
                smallest_index = i;
            }
        }

        std::swap(array[begin_index], array[smallest_index]);

        ++begin_index;
        smallest_index = begin_index;
    }

    return array;
}

int main() { assert(selectionSort({8, 5, 2, 9, 5, 6, 3}) == (std::vector<int>{2, 3, 5, 5, 6, 8, 9})); }
