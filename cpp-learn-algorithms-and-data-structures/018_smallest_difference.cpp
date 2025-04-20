#include <algorithm>
#include <cassert>
#include <vector>
#include <limits>

// 0(n^2) T, O(1) S
std::vector<int> smallestDifference(std::vector<int> arrayOne, std::vector<int> arrayTwo) {
    std::vector<int> pair(2);
    int smallestDiff = std::numeric_limits<int>::max();

    for (size_t i = 0; i < arrayOne.size(); ++i) {
        for (size_t j = 0; j < arrayTwo.size(); ++j) {
            int diff = abs(arrayOne[i] - arrayTwo[j]);

            if (diff < smallestDiff) {
                smallestDiff = diff;
                pair[0] = arrayOne[i];
                pair[1] = arrayTwo[j];
            }
        }
    }

    return pair;
}

// O(nlog(n) + mlog(m)) T, O(1) S
std::vector<int> smallestDifference2(std::vector<int> arrayOne, std::vector<int> arrayTwo) {
    std::vector<int> pair(2);

    sort(arrayOne.begin(), arrayOne.end());
    sort(arrayTwo.begin(), arrayTwo.end());

    size_t indexOne = 0;
    size_t indexTwo = 0;
    int smallestDiff = std::numeric_limits<int>::max();

    while (indexOne < arrayOne.size() && indexTwo < arrayTwo.size()) {
        int diff = abs(arrayOne[indexOne] - arrayTwo[indexTwo]);

        if (diff < smallestDiff) {
            smallestDiff = diff;
            pair[0] = arrayOne[indexOne];
            pair[1] = arrayTwo[indexTwo];
        }

        if (diff == 0) {
            break;
        }

        if (arrayOne[indexOne] < arrayTwo[indexTwo]) {
            ++indexOne;
        } else {
            ++indexTwo;
        }
    }

    return pair;
}

int main() {
    auto arrayOne = std::vector<int>{-1, 5, 10, 20, 28, 3};
    auto arrayTwo = std::vector<int>{26, 134, 135, 15, 17};
    auto expected = std::vector<int>{28, 26};

    assert(smallestDifference(arrayOne, arrayTwo) == expected);
    assert(smallestDifference2(arrayOne, arrayTwo) == expected);
}
