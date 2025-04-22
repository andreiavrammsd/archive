/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <vector>

template<typename T>
class Less_than {
    const T val; // value to compare agains

public:
    explicit Less_than(const T &v) : val(v) {}

    bool operator()(const T &x) const { // call operator
        return x < val;
    }
};

template<typename C, typename P>
// template function
int count(const C &c, P pred) {
    int cnt = 0;
    for (const auto &x : c) {
        if (pred(x)) {
            ++cnt;
        }
    }

    return cnt;
}

int main() {
    Less_than<int> lti{42};
    assert(lti(30));
    assert(!lti(42));

    std::vector<int> numbers = {2, 4, 88};
    assert(count(numbers, lti) == 2);
}
