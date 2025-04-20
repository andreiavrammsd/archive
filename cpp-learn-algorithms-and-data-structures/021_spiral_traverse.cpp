#include <cassert>
#include <vector>

// O(n) T, O(n) S
std::vector<int> spiralTraverse(std::vector<std::vector<int>> array) {
    std::vector<int> numbers;

    size_t startRow = 0;
    size_t endRow = array.size() - 1;
    size_t startCol = 0;
    size_t endCol = array[0].size() - 1;

    while (startRow <= endRow && startCol <= endCol) {
        for (size_t i = startCol; i <= endCol; ++i) {
            numbers.push_back(array[startRow][i]);
        }

        for (size_t i = startRow + 1; i <= endRow; ++i) {
            numbers.push_back(array[i][endCol]);
        }

        if (endCol == startCol) {
            break;
        }

        for (size_t i = endCol - 1; i > startCol; --i) {
            numbers.push_back(array[endRow][i]);
        }

        for (size_t i = endRow; i > startRow; --i) {
            numbers.push_back(array[i][startCol]);
        }

        ++startRow;
        ++startCol;
        --endRow;
        --endCol;
    }

    return numbers;
}

// O(n) T, O(n) S
void traverse(std::vector<std::vector<int>>& array, size_t startRow, size_t endRow, size_t startCol, size_t endCol,
              std::vector<int>& numbers) {
    if (startRow > endRow || startCol > endCol) {
        return;
    }

    for (size_t i = startCol; i <= endCol; ++i) {
        numbers.push_back(array[startRow][i]);
    }

    for (size_t i = startRow + 1; i <= endRow; ++i) {
        numbers.push_back(array[i][endCol]);
    }

    if (endCol == startCol) {
        return;
    }

    for (size_t i = endCol - 1; i > startCol; --i) {
        numbers.push_back(array[endRow][i]);
    }

    for (size_t i = endRow; i > startRow; --i) {
        numbers.push_back(array[i][startCol]);
    }

    ++startRow;
    ++startCol;
    --endRow;
    --endCol;

    traverse(array, startRow, endRow, startCol, endCol, numbers);
}

std::vector<int> spiralTraverse2(std::vector<std::vector<int>> array) {
    size_t startRow = 0;
    size_t endRow = array.size() - 1;
    size_t startCol = 0;
    size_t endCol = array[0].size() - 1;
    std::vector<int> numbers;

    traverse(array, startRow, endRow, startCol, endCol, numbers);

    return numbers;
}

int main() {
    struct test {
        std::vector<std::vector<int>> input;
        std::vector<int> expected;
    };
    auto tests = std::vector<test>{
        {
            std::vector<std::vector<int>>{
                {1},
            },
            {1},
        },
        {
            std::vector<std::vector<int>>{
                {1, 2, 3, 4},
                {12, 13, 14, 5},
                {11, 16, 15, 6},
                {10, 9, 8, 7},
            },
            {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
        },
        {
            std::vector<std::vector<int>>{
                {1, 2, 3, 4},
                {10, 11, 12, 5},
                {9, 8, 7, 6},
            },
            {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
        },
        {
            std::vector<std::vector<int>>{
                {1, 2, 3},
                {12, 13, 4},
                {11, 14, 5},
                {10, 15, 6},
                {9, 8, 7},
            },
            {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
        },
    };

    for (const auto& test : tests) {
        assert(spiralTraverse(test.input) == test.expected);
        assert(spiralTraverse2(test.input) == test.expected);
    }
}
