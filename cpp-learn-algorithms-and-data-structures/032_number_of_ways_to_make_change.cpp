#include <cassert>
#include <vector>
// O(nd) T | O(n) S
int numberOfWaysToMakeChange(int n, std::vector<int> denoms) {
    std::vector<int> change(n + 1, 0);
    change[0] = 1;

    for (int d : denoms) {
        for (int i = 1; i <= n; ++i) {
            if (d <= i) {
                change[i] += change[i - d];
            }
        }
    }

    return change.back();
}

int main() {
    assert(numberOfWaysToMakeChange(6, std::vector<int>{1, 5}) == 2);
    assert(numberOfWaysToMakeChange(4, std::vector<int>{1, 5, 10, 25}) == 1);
}
