#include <cassert>
#include <string>
#include <vector>

// O(nm) T | O(nm) S
int levenshteinDistance(std::string str1, std::string str2) {
    std::vector<std::vector<int>> ops(str2.size() + 1, std::vector<int>(str1.size() + 1, 0));

    for (size_t i = 0; i <= str2.size(); ++i) {
        for (size_t j = 0; j <= str1.size(); ++j) {
            ops[i][j] = j;
        }
        ops[i][0] = i;
    }

    for (size_t i = 1; i <= str2.size(); ++i) {
        for (size_t j = 1; j <= str1.size(); ++j) {
            if (str2[i - 1] == str1[j - 1]) {
                ops[i][j] = ops[i - 1][j - 1];
            } else {
                ops[i][j] = std::min(ops[i - 1][j - 1], std::min(ops[i][j - 1], ops[i - 1][j])) + 1;
            }
        }
    }

    return ops[str2.size()][str1.size()];
}

int main() {
    struct test {
        std::string a;
        std::string b;
        int distance;
    };
    std::vector<test> tests = {
        {
            "abc",
            "yabd",
            2,
        },
        {
            "abbbbbbbbb",
            "bbbbbbbbba",
            2,
        },
        {
            "table",
            "bal",
            3,
        },
        {
            "gumbo",
            "gambol",
            2,
        },
        {
            "xabc",
            "abcx",
            2,
        },
    };

    for (const auto& test : tests) {
        assert(levenshteinDistance(test.a, test.b) == test.distance);
    }
}
