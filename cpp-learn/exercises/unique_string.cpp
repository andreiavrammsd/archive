/*
 https://binarysearch.io/room/553

 Given a string s, determine whether it has all unique characters.
*/

#include <string>
#include <unordered_map>
#include <map>
#include <cassert>

bool uniqueString(const std::string &str) {
    std::unordered_map<char, bool> chars;

    for (const auto &c : str) {
        if (chars.find(c) != chars.end()) {
            return false;
        }

        chars[c] = true;
    }

    return true;
}

int main() {
    std::map<std::string, bool> strings = {
            {"",          true},
            {"aaa",       false},
            {"abcde",     true},
            {"abccde",    false},
            {"xcvsdfwer", true},
            {"aab",       false},
    };

    for (const auto &str : strings) {
        assert(uniqueString(str.first) == str.second);
    }
}
