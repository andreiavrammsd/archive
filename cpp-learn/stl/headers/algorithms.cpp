/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 */

#include <algorithm> // General algorithms
#include <cstdlib> // bsearch() , qsort()

// C++17
#include <execution> // Parallel execution policies

#include <vector>
#include <cassert>
#include <list>

int cmp(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

int main() {
    // General algorithms
    std::vector<int> va{1, 2};
    std::vector<int> vb(va.size());

    std::copy(va.begin(), va.end(), vb.begin());
    assert(va == vb);

    std::transform(vb.cbegin(), vb.cend(), vb.begin(), [](int x) { return x + x; });
    assert(vb == (std::vector<int>{2, 4}));

    std::list<int> list{2, 5, 3};
    auto p = std::find_if(list.cbegin(), list.cend(), [](int x) { return x > 10; });
    assert(p == list.cend());

    // bsearch(), qsort()
    int numbers[] = {340, 2, 1, 6, -2, 89};
    size_t size = 6;

    qsort(numbers, size, sizeof(int), cmp);

    int key = 89;
    int *item = (int *) bsearch(&key, numbers, size, sizeof(int), cmp);
    assert(item != nullptr);
    assert(*item == 89);

    // Parallel execution policies
    auto f = std::find(std::execution::par, va.cbegin(), va.cend(), 2);
    assert(f == va.cend() - 1);

    va = {2, 1};
    std::sort(std::execution::seq, std::begin(va), std::end(va));
    assert(va == (std::vector<int>{1, 2}));
}
