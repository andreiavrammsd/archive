#include <cassert>
#include <vector>

// O(log n) T, O(1) S
int binarySearch(std::vector<int> array, int target) {
    int left = 0;
    int right = array.size() - 1;
    int mid = (left + right) / 2;

    while (left <= right) {
        if (array[mid] == target) {
            return mid;
        }

        if (target > array[mid]) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }

        mid = (left + right) / 2;
    }

    return -1;
}

// O(log n) ST
int search(const std::vector<int>& array, int target, int left, int right) {
    if (left > right) {
        return -1;
    }

    int mid = (left + right) / 2;

    if (array[mid] == target) {
        return mid;
    }

    if (array[mid] > target) {
        right = mid - 1;
    } else {
        left = mid + 1;
    }

    return search(array, target, left, right);
}

int binarySearch2(const std::vector<int>& array, int target) { return search(array, target, 0, array.size() - 1); }

// O(log n) T, O(1) S
std::vector<int>::const_iterator binarySearch3(const std::vector<int>& array, int target) {
    auto b = array.cbegin();
    auto e = array.cend();
    auto mid = b + (e - b) / 2;

    while (b <= e) {
        if (*mid == target) {
            return mid;
        }

        if (target > *mid) {
            b = mid + 1;
        } else {
            e = mid - 1;
        }

        mid = b + (e - b) / 2;
    }

    return array.cend();
}

int main() {
    auto input = std::vector<int>{0, 1, 21, 33, 45, 45, 61, 71, 72, 73};
    assert(binarySearch(input, 33) == 3);
    assert(binarySearch2(input, 33) == 3);
    assert(binarySearch3(input, 33) == input.begin() + 3);
    assert(binarySearch3(input, 31) == input.end());
}
