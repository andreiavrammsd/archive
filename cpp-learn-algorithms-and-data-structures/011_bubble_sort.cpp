#include <cassert>
#include <vector>

// best: O(n) T, O(1) S
// avg: O(n^2) T, O(1) S
// worst: O(n^2) T, O(1) S
std::vector<int> bubbleSort(std::vector<int> array) {
    for (auto i = 0UL; i < array.size(); ++i) {
        for (auto j = 0UL; j < array.size(); ++j) {
            if (array[j] > array[i]) {
                std::swap(array[i], array[j]);
            }
        }
    }
    return array;
}

std::vector<int> bubbleSort2(std::vector<int> array) {
    auto isSorted = false;

    while (!isSorted) {
        isSorted = true;
        for (auto i = 0UL; i < array.size() - 1; ++i) {
            if (array[i] > array[i + 1]) {
                std::swap(array[i], array[i + 1]);
                isSorted = false;
            }
        }
    }

    return array;
}

std::vector<int> bubbleSort3(std::vector<int> array) {
    if (array.empty()) {
        return {};
    }

    auto isSorted = false;
    auto limit = array.size() - 1;

    while (!isSorted) {
        isSorted = true;
        for (auto i = 0UL; i < limit; ++i) {
            if (array[i] > array[i + 1]) {
                std::swap(array[i], array[i + 1]);
                isSorted = false;
            }
        }
        --limit;
    }

    return array;
}

int main() {
    assert(bubbleSort({8, 5, 2, 9, 5, 6, 3}) == (std::vector<int>{2, 3, 5, 5, 6, 8, 9}));
    assert(bubbleSort2({8, 5, 2, 9, 5, 6, 3}) == (std::vector<int>{2, 3, 5, 5, 6, 8, 9}));
    assert(bubbleSort3({8, 5, 2, 9, 5, 6, 3}) == (std::vector<int>{2, 3, 5, 5, 6, 8, 9}));
}
