#include <algorithm>
#include <cassert>
#include <unordered_set>
#include <vector>

std::vector<int> twoNumberSum(const std::vector<int>& array, int targetSum) {
    for (auto i = 0UL; i < array.size() - 1; ++i) {
        for (auto j = i + 1; j < array.size(); ++j) {
            if (array[i] + array[j] == targetSum) {
                return {array[i], array[j]};
            }
        }
    }
    return {};
}

std::vector<int> twoNumberSum2(const std::vector<int>& array, int targetSum) {
    std::unordered_set<int> numbers;

    for (auto n : array) {
        auto num = targetSum - n;

        if (num == targetSum) {
            return {n, num};
        }

        if (numbers.find(num) != numbers.end()) {
            return {n, num};
        }

        numbers.insert(n);
    }

    return {};
}

std::vector<int> twoNumberSum3(std::vector<int>& array, int targetSum) {
    sort(array.begin(), array.end());

    auto left = array.begin();
    auto right = array.end() - 1;

    while (left != right) {
        auto sum = *left + *right;

        if (sum == targetSum) {
            return {*left, *right};
        }

        if (sum < targetSum) {
            ++left;
        } else if (sum > targetSum) {
            --right;
        }
    }

    return {};
}

int main() {
    std::vector<int> array = {3, 5, -4, 8, 11, 1, -1};
    int targetSum = 10;

    assert(twoNumberSum(array, targetSum) == (std::vector<int>{11, -1}));
    assert(twoNumberSum2(array, targetSum) == (std::vector<int>{-1, 11}));
    assert(twoNumberSum3(array, targetSum) == (std::vector<int>{-1, 11}));
}
