#include <cassert>
#include <vector>

// O(n^2) T, O(n) S
std::vector<int> arrayOfProducts(std::vector<int> array) {
    std::vector<int> products;

    for (size_t i = 0; i < array.size(); ++i) {
        int product = 1;
        for (size_t j = 0; j < array.size(); ++j) {
            if (i != j) {
                product *= array[j];
            }
        }

        products.push_back(product);
    }

    return products;
}

// O(n^2) T, O(n) S
std::vector<int> arrayOfProducts2(std::vector<int> array) {
    std::vector<int> products;

    int total_product = 1;

    size_t i = 0;
    while (i < array.size()) {
        int product = total_product;

        size_t j = i + 1;
        while (j < array.size()) {
            product *= array[j];
            ++j;
        }

        products.push_back(product);
        total_product *= array[i];

        ++i;
    }

    return products;
}

// O(n^2) T, O(n) S
std::vector<int> arrayOfProducts3(std::vector<int> array) {
    std::vector<int> products;

    int product = 1;
    for (int n : array) {
        products.push_back(product);
        product *= n;
    }

    product = 1;
    for (size_t i = array.size(); i != 0; --i) {
        products[i - 1] *= product;
        product *= array[i - 1];
    }

    return products;
}

int main() {
    struct test {
        std::vector<int> input;
        std::vector<int> expected;
    };
    auto tests = std::vector<test>{
        {
            std::vector<int>{5, 1, 4, 2},
            std::vector<int>{8, 40, 10, 20},
        },

        {
            std::vector<int>{0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
            std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        },

        {
            std::vector<int>{9, 3, 2, 1, 9, 5, 3, 2},
            std::vector<int>{1620, 4860, 7290, 14580, 1620, 2916, 4860, 7290},
        },
    };

    for (const auto& test : tests) {
        assert(arrayOfProducts(test.input) == test.expected);
        assert(arrayOfProducts2(test.input) == test.expected);
        assert(arrayOfProducts3(test.input) == test.expected);
    }
}
