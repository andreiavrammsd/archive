#include <cassert>
#include <vector>

// O(n) T, O(1) S
int longestPeak(std::vector<int> array) {
    if (array.size() < 3) {
        return 0;
    }

    int length = 0;

    size_t i = 1;
    while (i < array.size() - 1) {
        int prev = array[i - 1];
        int curr = array[i];
        int next = array[i + 1];

        if (curr > prev && curr > next) {
            size_t left = i - 1;
            while (left > 0 && array[left] > array[left - 1]) {
                --left;
            }

            size_t right = i + 1;
            while (right < array.size() && array[right] < array[right - 1]) {
                ++right;
            }

            int current_length = static_cast<int>(right - left);
            if (current_length > length) {
                length = current_length;
                i = right;
            } else {
                i = right + 1;
            }
        } else {
            ++i;
        }
    }

    return length;
}

int main() {
    struct test {
        std::vector<int> input;
        int expected;
    };
    auto tests = std::vector<test>{
        {
            std::vector<int>{1, 2, 3, 3, 4, 0, 10},
            3,
        },

        {
            std::vector<int>{1, 2, 3, 2, 1, 1},
            5,
        },

        {
            std::vector<int>{5, 4, 3, 2, 1, 2, 1},
            3,
        },

        {
            std::vector<int>{1, 2, 3, 4, 5, 1},
            6,
        },

        {
            std::vector<int>{1, 3, 2},
            3,
        },

        {
            std::vector<int>{1, 2, 3, 3, 4, 0, 10, 6, 5, -1, -3, 2, 3},
            6,
        },

        {
            std::vector<int>{1, 1, 1, 2, 3, 10, 12, -3, -3, 2, 3, 45, 800, 99, 98, 0, -1, -1, 2, 3, 4, 5, 0, -1, -1},
            9,
        },
    };

    for (const auto& test : tests) {
        assert(longestPeak(test.input) == test.expected);
    }
}
