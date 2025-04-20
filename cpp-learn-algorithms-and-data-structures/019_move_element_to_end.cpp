#include <cassert>
#include <vector>

// O(n) T, O(1) S
std::vector<int> moveElementToEnd(std::vector<int> array, int toMove) {
    size_t pos = 0;

    for (auto i = 0UL; i < array.size(); ++i) {
        if (array[i] != toMove) {
            std::swap(array[i], array[pos]);
            ++pos;
        }
    }

    return array;
}

int main() { assert(moveElementToEnd({2, 1, 2, 2, 2, 3, 4, 2}, 2) == (std::vector<int>{1, 3, 4, 2, 2, 2, 2, 2})); }
