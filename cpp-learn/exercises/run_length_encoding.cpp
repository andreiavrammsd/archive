/*
 Run-length encoding is a fast and simple method of encoding strings.
 The basic idea is to represent repeated successive characters as a single count and character.
 For example, the string "AAAABBBCCDAA" would be encoded as "4A3B2C1D2A".

 Implement run-length encoding. You can assume the string to be encoded have no digits and consists solely
 of alphabetic characters.

 Examples:
    "AAAABBBCCDAA" -> "4A3B2C1D2A"
    "AABBA" -> "2A2B1A"
    "ABCDE" -> "1A1B1C1D1E"
*/

#include <map>
#include <cassert>
#include <iostream>

std::string encode(const std::string &input) {
    if (input.empty()) {
        return "";
    }

    char character{};
    int count{};
    std::string encoded;

    for (const char &i : input) {
        if (character && character != i) {
            encoded += std::to_string(count) + character;
            count = 0;
        }

        character = i;
        count++;
    }

    encoded += std::to_string(count) + character;

    return encoded;
}

int main() {
    std::map<std::string, std::string> tests = {
            {"",             ""},
            {" ",            "1 "},
            {"AAAABBBCCDAA", "4A3B2C1D2A"},
            {"AABBA",        "2A2B1A"},
            {"ABCDE",        "1A1B1C1D1E"},
    };

    for (const auto &t : tests) {
        auto encoded = encode(t.first);
        std::cout << t.first << " -> " << encoded << std::endl;
        assert(encoded == t.second);
    }
}
