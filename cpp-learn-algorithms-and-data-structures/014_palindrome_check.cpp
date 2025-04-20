#include <cassert>
#include <string>

// O(n) T, O(1) S
bool isPalindrome(std::string str) {
    size_t left = 0;
    size_t right = str.size() - 1;

    while (left < right) {
        if (str[left] != str[right]) {
            return false;
        }

        ++left;
        --right;
    }

    return true;
}

// O(n) T, O(n) S - could be O(1) S because of tail recursion
bool isSubPalindrome(std::string str, size_t first, size_t last) {
    if (first >= last) {
        return true;
    }

    if (str[first] != str[last]) {
        return false;
    }

    return isSubPalindrome(str, first + 1, last - 1);
}

bool isPalindrome2(std::string str) { return isSubPalindrome(str, 0, str.size() - 1); }

int main() {
    assert(isPalindrome("abcdcba"));
    assert(isPalindrome("abba"));
    assert(!isPalindrome("ab"));

    assert(isPalindrome2("abcdcba"));
    assert(isPalindrome2("abba"));
    assert(!isPalindrome2("ab"));
}
