/*
 The C++ Programming Language, 4th Edition
 */

#include <stack>
#include <cassert>

int main() {
    std::stack<int> s;
    s.push(2);
    s.emplace(1);

    assert(!s.empty());
    assert(s.size() == 2);

    s.pop();
    assert(s.size() == 1);
    assert(s.top() == 2);

    std::stack<int> s2;
    s2.push(2);
    assert(s2 == s);
}
