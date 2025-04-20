#include <cassert>
#include <vector>

// O(n) T | O(1) S
int kadanesAlgorithm(std::vector<int> array) {
    int currentSum = array[0];
    int sum = array[0];

    for (size_t i = 1; i < array.size(); ++i) {
        currentSum = std::max(array[i], currentSum + array[i]);
        sum = std::max(sum, currentSum);
    }

    return sum;
}

int main() {
    struct test {
        std::vector<int> input;
        int expected;
    };
    std::vector<test> tests = {
        {
            {3, 4, -6, 7, 8},
            16,
        },
        {
            {3, 5, -9, 1, 3, -2, 3, 4, 7, 2, -9, 6, 3, 1, -5, 4},
            19,
        },
    };

    for (const auto& test : tests) {
        assert(kadanesAlgorithm(test.input) == test.expected);
    }
}
