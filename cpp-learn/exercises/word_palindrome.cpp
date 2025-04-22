#include <string>
#include <cassert>

bool wordIsPalindrome(const std::string &phrase) {
    std::size_t length = phrase.length();

    for (size_t i = 0; i < length / 2; i++) {
        if (std::tolower(phrase[i]) != std::tolower(phrase[length - i - 1])) {
            return false;
        }
    }

    return true;
}

int main() {
    std::string palindromeWords =
            "Anna\n"
            "Civic\n"
            "Kayak\n"
            "Level\n"
            "Madam\n"
            "Mom\n"
            "Noon\n"
            "Racecar\n"
            "Radar\n"
            "Redder\n"
            "Refer\n"
            "Repaper\n"
            "Rotator\n"
            "Rotor\n"
            "Sagas\n"
            "Solos\n"
            "Stats\n"
            "Tenet\n"
            "Wow\n";

    std::string word;

    for (const char &c : palindromeWords) {
        if (c == '\n') {
            assert(wordIsPalindrome(word));
            word = "";
        } else {
            word += c;
        }
    }
}
