#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

// O(n log n) T, O(1) S
std::vector<int> findThreeLargestNumbers(std::vector<int> array) {
    std::vector<int> largest;

    // O(n) T
    for (auto n : array) {
        if (largest.size() < 3) {
            largest.push_back(n);
            continue;
        }

        // O(n log n) T
        sort(largest.begin(), largest.end());

        if (n > largest.front()) {
            largest.front() = n;
        }
    }

    // O(n log n) T
    sort(largest.begin(), largest.end());

    return largest;
}

// O(n) T, O(1) S
std::vector<int> findThreeLargestNumbers2(std::vector<int> array) {
    std::vector<int> largest;

    // O(n) T
    for (auto n : array) {
        if (largest.size() < 3) {
            largest.push_back(n);
            continue;
        }

        // O(n) T
        auto smallest_index = 0;
        for (auto i = 1UL; i < largest.size(); ++i) {
            if (largest[i] < largest[smallest_index]) {
                smallest_index = i;
            }
        }

        if (n > largest[smallest_index]) {
            largest[smallest_index] = n;
        }
    }

    // O(n log n) T
    sort(largest.begin(), largest.end());

    return largest;
}

// O(n) T, O(1) S
void addLargest(std::vector<int>& largest, int n, size_t index) {
    for (auto i = 0UL; i < index; ++i) {
        largest[i] = largest[i + 1];
    }

    largest[index] = n;
}

std::vector<int> findThreeLargestNumbers3(std::vector<int> array) {
    std::vector<int> largest(3, std::numeric_limits<int>::min());

    for (auto n : array) {
        for (auto i = largest.size(); i != 0; --i) {
            if (n > largest[i - 1]) {
                addLargest(largest, n, i - 1);
                break;
            }
        }
    }

    return largest;
}

int main() {
    assert(findThreeLargestNumbers({141, 1, 17, -7, -17, -27, 18, 541, 8, 7, 7}) == (std::vector<int>{18, 141, 541}));
    assert(findThreeLargestNumbers({-1, -2, -3, -7, -17, -27, -18, -541, -8, -7, 7}) == (std::vector<int>{-2, -1, 7}));

    assert(findThreeLargestNumbers2({141, 1, 17, -7, -17, -27, 18, 541, 8, 7, 7}) == (std::vector<int>{18, 141, 541}));
    assert(findThreeLargestNumbers2({-1, -2, -3, -7, -17, -27, -18, -541, -8, -7, 7}) == (std::vector<int>{-2, -1, 7}));

    assert(findThreeLargestNumbers3({141, 1, 17, -7, -17, -27, 18, 541, 8, 7, 7}) == (std::vector<int>{18, 141, 541}));
    assert(findThreeLargestNumbers3({-1, -2, -3, -7, -17, -27, -18, -541, -8, -7, 7}) == (std::vector<int>{-2, -1, 7}));
}
