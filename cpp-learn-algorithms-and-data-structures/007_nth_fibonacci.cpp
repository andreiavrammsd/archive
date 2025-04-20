#include <cassert>
#include <unordered_map>
// O(n) T, O(1) S
int getNthFib(int n) {
    if (n == 1) return 0;

    int a = 0;
    int b = 1;

    while (n - 2 > 0) {
        b = b + a;
        a = b - a;
        --n;
    }

    return b;
}

// O(2^n) T, O(n) S
int getNthFib2(int n) {
    if (n == 1) return 0;
    if (n == 2) return 1;

    return getNthFib2(n - 1) + getNthFib2(n - 2);
}

// O(n) ST
int fib(int n, std::unordered_map<int, int> &memo) {
    auto f = memo.find(n);
    if (f != memo.end()) {
        return f->second;
    }

    memo.emplace(n, fib(n - 1, memo) + fib(n - 2, memo));

    return memo[n];
}

int getNthFib3(int n) {
    std::unordered_map<int, int> memo = {{1, 0}, {2, 1}};
    return fib(n, memo);
}

int main() {
    assert(getNthFib(6) == 5);
    assert(getNthFib2(6) == 5);
    assert(getNthFib3(6) == 5);
}
