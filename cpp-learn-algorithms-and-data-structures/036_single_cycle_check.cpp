#include <cassert>
#include <vector>
// O(n) T | O(1) S
bool hasSingleCycle(std::vector<int> array) {
    int index = 0;
    int visited = 0;
    const auto size = static_cast<int>(array.size());

    while (visited < size) {
        if (index == 0 && visited > 0) {
            return false;
        }
        ++visited;

        index = (index + array[index]) % size;
        if (index < 0) {
            index += size;
        }
    }

    return index == 0;
}

int main() {
    struct test {
        std::vector<int> input;
        bool expected;
    };
    std::vector<test> tests = {
        {
            {1, 2, 3, 4, -2, 3, 7, 8, -26},
            true,
        },
        {
            {2, 2, -1},
            true,
        },
        {
            {2, 3, 1, -4, -4, 2},
            true,
        },
        {
            {0, 1, 1, 1, 1},
            false,
        },
    };

    for (const auto& test : tests) {
        assert(hasSingleCycle(test.input) == test.expected);
    }
}
