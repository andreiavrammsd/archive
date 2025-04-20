#include <cassert>
#include <limits>
#include <vector>
// O(nd) T | O(n) S
int minNumberOfCoinsForChange(int n, std::vector<int> denoms) {
    int max = std::numeric_limits<int>::max();
    std::vector<int> ways(n + 1, max);
    ways[0] = 0;

    for (auto denom : denoms) {
        for (int sum = 1; sum <= n; ++sum) {
            if (denom <= sum) {
                int w = ways[sum - denom];
                if (w < ways[sum]) {
                    ways[sum] = w + 1;
                }
            }
        }
    }

    return ways.back() == max ? -1 : ways.back();
}

int main() {
    assert(minNumberOfCoinsForChange(6, std::vector<int>{1, 2, 4}) == 2);
    assert(minNumberOfCoinsForChange(7, std::vector<int>{1, 5, 10}) == 3);
    assert(minNumberOfCoinsForChange(9, std::vector<int>{3, 5}) == 3);
}
