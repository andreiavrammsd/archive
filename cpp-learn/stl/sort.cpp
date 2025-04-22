/*
 The C++ Programming Language, 4th Edition
 */

#include <vector>
#include <string>
#include <cassert>
#include <algorithm>

template<typename T, typename Compare = std::less<T>>
void sort(std::vector<T> &);

void f(std::vector<int> &vi, std::vector<std::string> &vs) {
    sort(vi); // sort(std::vector<int>&);
    sort(vs); // sort(std::vector<std::string>&);
}

template<typename T, typename Compare>
void sort(std::vector<T> &v) { // Shell sort (Knuth, Vol. 3, pg. 84)
    Compare cmp; // make a default Compare object
    const size_t n = v.size();

    for (int gap = n / 2; 0 < gap; gap /= 2) {
        for (size_t i = gap; i < n; i++) {
            for (int j = static_cast<int>(i) - gap; 0 <= j; j -= gap) {
                if (cmp(v[j + gap], v[j])) {
                    std::swap(v[j], v[j + gap]);
                }
            }
        }
    }
}

struct No_case {
    bool operator()(const std::string &x, const std::string &y) const {
        // return true if x is lexicographically less than y, not taking case into account

        auto p = x.begin();
        auto q = y.begin();

        while (p != x.end() && q != y.end() && std::toupper(*p) == std::toupper(*q)) {
            ++p;
            ++q;
        }

        if (p == x.end()) {
            return q != y.end();
        }

        if (q == y.end()) {
            return false;
        }

        return std::toupper(*p) < std::toupper(*q);
    }
};

struct No_case2 {
    bool operator()(const std::string &a, const std::string &b) const {
        std::string al;
        al.resize(a.length());
        std::transform(a.begin(), a.end(), al.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        std::string bl;
        bl.resize(b.length());
        std::transform(b.begin(), b.end(), bl.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        return al < bl;
    }
};

int main() {
    std::vector<int> v{1, 3, 2};

    sort(v);
    std::vector<int> v2{1, 2, 3};
    assert(v == v2);

    std::vector<std::string> s{"F", "abc", "zde"};

    sort<std::string, std::greater<std::string>>(s);
    std::vector<std::string> s2{"zde", "abc", "F"};
    assert(s == s2);

    sort<std::string, No_case>(s);
    std::vector<std::string> s3{"abc", "F", "zde"};
    assert(s == s3);

    s = {"F", "abc", "zde"};
    sort<std::string, No_case2>(s);
    assert(s == s3);
}
