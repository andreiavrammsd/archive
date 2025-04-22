/*
 https://www.dailycodingproblem.com/

 This problem was asked by Uber.

 Given an array of integers, return a new array such that each element at index i of the
 new array is the product of all the numbers in the original array except the one at i.

 For example, if our input was [1, 2, 3, 4, 5], the expected output would be [120, 60, 40, 30, 24].
 If our input was [3, 2, 1], the expected output would be [2, 3, 6].
 */

#include <numeric>
#include <vector>
#include <map>
#include <cassert>

std::vector<int> products(const std::vector<int> &numbers) {
    std::vector<int> results;
    results.resize(results.size());

    auto total = std::accumulate(numbers.begin(), numbers.end(), 1, std::multiplies<void>());

    for (int num : numbers) {
        results.push_back(total / num);
    }

    return results;
}

int main() {
    std::map<std::vector<int>, std::vector<int>> numbers = {
            {{1, 2, 3, 4, 5}, {120, 60, 40, 30, 24}},
            {{3, 2, 1},       {2,   3,  6}},
    };

    for (const auto &nums : numbers) {
        assert(products(nums.first) == nums.second);
    }
}
