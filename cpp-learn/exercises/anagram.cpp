#include <string>
#include <iostream>
#include <bits/stdc++.h>
#include <cassert>

bool wordsAreAnagram(const std::string &a, const std::string &b) {
    std::string lowerB;
    lowerB.resize(b.length());

    std::transform(b.begin(), b.end(), lowerB.begin(), ::tolower);

    for (const auto &c : a) {
        if (lowerB.find_first_of(std::tolower(c)) == std::string::npos) {
            return false;
        }
    }

    return true;
}

int main() {
    std::map<std::string, std::string> anagrams = {
            {"Army",            "Mary"},
            {"Cider",           "Cried"},
            {"Stressed",        "Desserts"},
            {"School master",   "The classroom"},
            {"The eyes",        "They see"},
            {"The Morse Code",  "Here comes dots"},
            {"Eleven plus two", "Twelve plus one"},
            {"Slot machines",   "Cash lost in me"},
            {"Fourth of July",  "Joyful Fourth"},
    };

    for (const auto &anagram : anagrams) {
        std::cout << anagram.first << " is " << anagram.second << std::endl;
        assert(wordsAreAnagram(anagram.first, anagram.second));
    }
}
