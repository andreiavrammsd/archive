/*
 The C++ Programming Language, 4th Edition
 */

#include <vector>
#include <cassert>

int main() {
    std::vector<int> v{1, 2, 3};

    auto it = v.cbegin();

    // A ++p returns a reference to p, whereas p++ must return a copy of p holding the old value. Thus, for
    // more complicated iterators, ++p is likely to be more efficient than p++.
    ++it;
    assert(*it == 2);
    assert(it == v.cbegin() + 1);

    --it;
    assert(*it == 1);
    assert(it == v.cbegin());

    auto val = *it;
    assert(val == 1);

    val = v[1];
    assert(val == 2);

    /*
     * ->
     * ==
     * !=
     * <
     * <=
     * >
     * >=
     * +=
     * -=
     * +
     * -
     */

    advance(it, 2);
    assert(*it == 3);

    assert(distance(it, v.cend()) == 1);

    auto p = prev(it);
    assert(*p == 2);
    assert(p == v.cbegin() + 1);

    --it;
    assert(*next(it) == 3);
}
