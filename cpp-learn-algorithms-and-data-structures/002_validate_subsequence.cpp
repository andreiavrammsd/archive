#include <cassert>
#include <unordered_map>
#include <vector>

// O(n^2) T, O(1) S
bool isValidSubsequence(std::vector<int> array, const std::vector<int>& sequence) {
    auto count = 0UL;
    auto pos = 0UL;

    for (auto n : sequence) {
        for (auto i = pos; i < array.size(); ++i) {
            if (n == array[i]) {
                pos = i + 1;
                ++count;
                break;
            }
        }
    }

    return count == sequence.size();
}

// O(n) ST
bool isValidSubsequence2(std::vector<int> array, std::vector<int> sequence) {
    std::unordered_map<int, size_t> seq;
    for (auto i = 0UL; i < sequence.size(); ++i) {
        seq[sequence[i]] = i;
    }

    auto count = 0UL;

    for (auto i = 0UL; i < array.size(); ++i) {
        auto s = seq.find(array[i]);

        if (s != seq.end() && i >= s->second) {
            ++count;
        }
    }

    return count == sequence.size();
}

// O(n) T, O(1) S
bool isValidSubsequence3(std::vector<int> array, std::vector<int> sequence) {
    auto seq = sequence.cbegin();
    for (auto n : array) {
        if (*seq == n) {
            ++seq;
        }

        if (seq == sequence.cend()) {
            return true;
        }
    }

    return false;
}

int main() {
    std::vector<int> array = {5, 1, 22, 25, 6, -1, 8, 10};
    std::vector<int> sequence = {1, 6, -1, 10};

    assert(isValidSubsequence(array, sequence));
    assert(isValidSubsequence2(array, sequence));
    assert(isValidSubsequence3(array, sequence));
}
