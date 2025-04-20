#include <cassert>
#include <vector>
// O(n) TS
int maxSubsetSumNoAdjacent(std::vector<int> array) {
    if (array.empty()) {
        return 0;
    }
    if (array.size() == 1) {
        return array[0];
    }

    std::vector<int> sums(array.size());
    sums[0] = array[0];
    sums[1] = std::max(array[0], array[1]);

    for (size_t i = 2; i < array.size(); ++i) {
        sums[i] = std::max(sums[i - 1], sums[i - 2] + array[i]);
    }

    return sums.back();
}

// O(n) T | O(1) S
int maxSubsetSumNoAdjacent2(std::vector<int> array) {
    if (array.empty()) {
        return 0;
    }
    if (array.size() == 1) {
        return array[0];
    }

    int a = array[0];
    int b = std::max(array[0], array[1]);

    for (size_t i = 2; i < array.size(); ++i) {
        int sum = std::max(b, a + array[i]);
        a = b;
        b = sum;
    }

    return b;
}

int main() {
    assert(maxSubsetSumNoAdjacent(std::vector<int>{75, 105, 120, 75, 90, 135}) == 330);
    assert(maxSubsetSumNoAdjacent2(std::vector<int>{75, 105, 120, 75, 90, 135}) == 330);
}
