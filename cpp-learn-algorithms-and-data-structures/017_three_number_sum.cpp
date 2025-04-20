#include <algorithm>
#include <cassert>
#include <vector>

// O(n^3) T, O(n)
std::vector<std::vector<int>> threeNumberSum(std::vector<int> array, int targetSum) {
    std::vector<std::vector<int>> numbers;
    sort(array.begin(), array.end());

    for (size_t i = 0; i < array.size(); ++i) {
        auto a = array[i];

        for (size_t j = i + 1; j < array.size(); ++j) {
            auto b = array[j];

            for (size_t k = j + 1; k < array.size(); ++k) {
                auto c = array[k];

                if (a + b + c == targetSum) {
                    numbers.push_back({a, b, c});
                }
            }
        }
    }

    return numbers;
}

// O(n^2) T, O(n)
std::vector<std::vector<int>> threeNumberSum2(std::vector<int> array, int targetSum) {
    std::vector<std::vector<int>> numbers;
    sort(array.begin(), array.end());

    for (size_t i = 0; i < array.size() - 1; ++i) {
        auto a = i;
        auto b = i + 1;
        auto c = array.size() - 1;

        for (size_t j = i + 1; j < array.size() - 1; ++j) {
            auto sum = array[a] + array[b] + array[c];

            if (sum == targetSum) {
                numbers.push_back({array[a], array[b], array[c]});
                ++b;
            } else if (sum < targetSum) {
                ++b;
            } else {
                --c;
            }
        }
    }

    return numbers;
}

// O(n^2) T, O(n)
std::vector<std::vector<int>> threeNumberSum3(std::vector<int> array, int targetSum) {
    std::vector<std::vector<int>> numbers;
    sort(array.begin(), array.end());

    for (size_t i = 0; i < array.size() - 2; ++i) {
        auto a = i;
        auto b = i + 1;
        auto c = array.size() - 1;

        while (b < c) {
            auto sum = array[a] + array[b] + array[c];

            if (sum == targetSum) {
                numbers.push_back({array[a], array[b], array[c]});
                ++b;
                --c;
            } else if (sum < targetSum) {
                ++b;
            } else {
                --c;
            }
        }
    }

    return numbers;
}

int main() {
    auto input = std::vector<int>{12, 3, 1, 2, -6, 5, -8, 6};
    auto output = std::vector<std::vector<int>>{{-8, 2, 6}, {-8, 3, 5}, {-6, 1, 5}};

    assert(threeNumberSum(input, 0) == output);
    assert(threeNumberSum2(input, 0) == output);
    assert(threeNumberSum3(input, 0) == output);
}
