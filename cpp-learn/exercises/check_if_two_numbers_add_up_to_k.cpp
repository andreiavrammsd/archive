/*
 https://www.dailycodingproblem.com/

 Given a list of numbers and a number k, return whether any two numbers from the list add up to k.
 For example, given [10, 15, 3, 7] and k of 17, return true since 10 + 7 is 17.
 */

#include <vector>
#include <map>
#include <unordered_set>
#include <cassert>

bool checkNumbers(const std::vector<int> &numbers, int k) {
    for (auto itn = numbers.cbegin(); itn != numbers.cend(); ++itn) {
        for (auto itm = itn + 1; itm != numbers.cend(); ++itm) {
            if (*itn + *itm == k) {
                return true;
            }
        }
    }

    return false;
}

bool checkNumbersSinglePass(const std::vector<int> &numbers, int k) {
    std::map<int, bool> aux;

    for (auto num : numbers) {
        auto n = k - num;
        if (aux.find(n) != aux.end()) {
            return true;
        }

        aux[num] = true;
    }

    return false;
}

bool checkNumbersSinglePassWithSet(const std::vector<int> &numbers, int k) {
    std::unordered_set<int> aux;

    for (auto num : numbers) {
        auto n = k - num;
        if (aux.find(n) != aux.end()) {
            return true;
        }

        aux.insert(num);
    }

    return false;
}

int main() {
    const std::vector<int> numbers = {10, 15, 3, 7};

    assert(checkNumbers(numbers, 17));
    assert(checkNumbers(numbers, 13));
    assert(!checkNumbers(numbers, 14));

    assert(checkNumbersSinglePass(numbers, 17));
    assert(checkNumbersSinglePass(numbers, 13));
    assert(!checkNumbersSinglePass(numbers, 14));

    assert(checkNumbersSinglePassWithSet(numbers, 17));
    assert(checkNumbersSinglePassWithSet(numbers, 13));
    assert(!checkNumbersSinglePassWithSet(numbers, 14));
}
