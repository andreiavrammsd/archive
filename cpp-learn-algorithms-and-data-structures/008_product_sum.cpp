#include <any>
#include <cassert>
#include <vector>

// O(n) T, O(d) S
int sumOfDepth(const std::vector<std::any>& array, int depth = 1) {
    int sum = 0;

    for (auto el : array) {
        if (el.type() == typeid(int)) {
            sum += std::any_cast<int>(el);
        }

        if (el.type() == typeid(std::vector<std::any>)) {
            sum += sumOfDepth(std::any_cast<const std::vector<std::any>&>(el), depth + 1);
        }
    }

    return depth * sum;
}

int productSum(std::vector<std::any> array) { return sumOfDepth(array); }

// O(n) T, O(d) S
int productSum2(const std::vector<std::any>& array, int depth = 1) {
    int sum = 0;

    for (const auto& element : array) {
        if (element.type() == typeid(std::vector<std::any>)) {
            sum += productSum2(std::any_cast<std::vector<std::any>>(element), depth + 1);
        } else {
            sum += std::any_cast<int>(element);
        }
    }

    return sum * depth;
}

int main() {
    auto input = std::vector<std::any>{{5, 2, std::vector<std::any>{7, -1}, 3, std::vector<std::any>{6, std::vector<std::any>{-13, 8}, 4}}};
    assert(productSum(input) == 12);
    assert(productSum2(input) == 12);
}
