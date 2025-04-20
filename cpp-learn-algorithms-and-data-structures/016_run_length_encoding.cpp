#include <cassert>
#include <string>
#include <vector>

// O(n) T, O(s) S
std::string runLengthEncoding(std::string str) {
    int n = 1;
    std::vector<char> encoded;

    for (size_t i = 1; i < str.size(); ++i) {
        char curr = str[i];
        char prev = str[i - 1];

        if (prev != curr || n == 9) {
            encoded.push_back(std::to_string(n)[0]);
            encoded.push_back(prev);
            n = 0;
        }

        ++n;
    }

    encoded.push_back(std::to_string(n)[0]);
    encoded.push_back(str[str.size() - 1]);

    return std::string(encoded.begin(), encoded.end());
}

int main() { assert(runLengthEncoding("AAAAAAAAAAAAABBCCCCDD") == "9A4A2B4C2D"); }
