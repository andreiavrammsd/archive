#include <cassert>
#include <string>

// O(n) T, O(s) S
std::string caesarCypherEncryptor(std::string str, int key) {
    std::string encrypted;
    encrypted.reserve(str.size());

    const char begin = 'a';
    const char end = 'z';
    const int numberOfChars = end - begin + 1;

    auto steps = key % numberOfChars;

    for (auto chr : str) {
        auto ch = chr + steps;

        if (ch > end) {
            ch -= numberOfChars;
        }

        encrypted.push_back(ch);
    }

    return encrypted;
}

// O(n) T, O(s) S
std::string caesarCypherEncryptor2(std::string str, int key) {
    std::string encrypted;
    encrypted.reserve(str.size());

    key %= 26;

    for (auto chr : str) {
        auto newChr = chr + key;

        if (newChr > 122) {
            newChr = 96 + newChr % 122;
        }

        encrypted.push_back(newChr);
    }

    return encrypted;
}

int main() {
    assert(caesarCypherEncryptor("xyz", 2) == "zab");
    assert(caesarCypherEncryptor("abc", 57) == "fgh");
    assert(caesarCypherEncryptor2("xyz", 2) == "zab");
    assert(caesarCypherEncryptor2("abc", 57) == "fgh");
}
