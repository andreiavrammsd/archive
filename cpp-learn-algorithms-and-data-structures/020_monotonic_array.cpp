#include <cassert>
#include <vector>

// O(n) T, O(1) S
bool isMonotonic(std::vector<int> array) {
    int state = 0;
    size_t i = 1UL;

    while (i < array.size()) {
        if (array[i - 1] > array[i]) {
            if (state == 1) {
                return false;
            }

            state = -1;
        } else if (array[i - 1] < array[i]) {
            if (state == -1) {
                return false;
            }

            state = 1;
        }

        ++i;
    }

    return true;
}

int main() {
    assert(isMonotonic({-1, -5, -10, -1100, -1100, -1101, -1102, -9001}));
    assert(!isMonotonic({-1, -5, -10, -1100, -900, -1101, -1102, -9001}));
}
