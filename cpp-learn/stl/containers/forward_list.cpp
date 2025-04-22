/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <forward_list>
#include <iostream>

template <typename T>
size_t size(const std::forward_list<T> &list)
{
    size_t size = 0;
    auto it = list.begin();
    while (it != list.end()) {
        ++size;
        ++it;
    }
    return size;
}

template <typename Iterator>
void print(const Iterator it)
{
    for (const auto i : it) {
        std::cout << i;
    }
    std::cout << '\n';
}

int main()
{
    std::forward_list<int> list;

    list.push_front(1);
    list.emplace_front(2);
    assert(size(list) == 2);

    std::forward_list<int> list2{3, 4};
    list.merge(list2);
    assert(size(list) == 4);
    print(list);

    auto pos = list.before_begin();
    ++pos;
    ++pos;
    list.insert_after(pos, 9);
    print(list);

    list2.push_front(9);
    list2.push_front(7);

    auto p = list.begin();
    ++p;
    assert(*p == 1);  // not 2

    auto q = list2.begin();
    ++q;
    assert(*q == 9);

    // A splice() operation does not copy element values_ and does not invalidate iterators to elements.
    list.splice_after(pos, list2);
    assert(*p == 1);
    assert(*q == 9);
    print(list);

    ++p;
    list.erase_after(p);
    print(list);
}
