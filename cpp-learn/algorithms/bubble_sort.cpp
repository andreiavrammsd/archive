#include <iostream>
#include <algorithm>
#include <cassert>
#include <array>

#define ARRAY_TPL template<typename T, std::size_t S>

ARRAY_TPL
void sort(std::array<T, S> &numbers) {
    for (size_t i = 0; i < numbers.size(); i++) {
        for (size_t j = 0; j < numbers.size(); j++) {
            if (numbers[i] < numbers[j]) {
                std::swap(numbers[i], numbers[j]);
            }
        }
    }
}

ARRAY_TPL
void print(std::array<T, S> numbers) {
    for (const auto &n : numbers) {
        std::cout << n << std::endl;
    }

    std::cout << std::endl;
}

int main() {
    // integers
    std::array<int, 5> ints = {1, 6, -7, 3, 45};
    auto intsCopy = ints;

    sort(ints);
    print(ints);

    std::sort(std::begin(intsCopy), std::end(intsCopy));
    print(intsCopy);

    assert(ints == intsCopy);

    // doubles
    std::array<double, 5> doubles = {12, 4.6, -12.2, -12.1, 0.9};
    auto doublesCopy = doubles;

    sort(doubles);
    print(doubles);

    std::sort(std::begin(doublesCopy), std::end(doublesCopy));
    print(doublesCopy);

    assert(doubles == doublesCopy);

    // chars
    std::array<char, 5> chars = {'B', 'Z', 'G', 'C', 'A'};
    auto charsCopy = chars;

    sort(chars);
    print(chars);

    std::sort(std::begin(charsCopy), std::end(charsCopy));
    print(charsCopy);

    assert(chars == charsCopy);

    // strings
    std::array<std::string, 4> words = {"brown", "jumps", "a", "fox"};
    auto wordsCopy = words;

    sort(words);
    print(words);

    std::sort(wordsCopy.begin(), std::end(wordsCopy));
    print(wordsCopy);

    assert(words == wordsCopy);
}
