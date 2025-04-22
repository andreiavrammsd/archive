/*
 The C++ Programming Language, 4th Edition
 */

#include <deque>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
    std::deque<int> queue;

    queue.emplace_back(1);
    queue.emplace_front(2);

    assert(queue.front() == 2);
    assert(queue.back() == 1);
    assert(queue.size() == 2);

    assert(queue[0] == 2);
    queue[0] = 3;
    assert(queue[0] == 3);
    assert(queue.front() == 3);

    std::vector<int> data = {4, 5, 6};
    queue.insert(queue.begin(), data.begin(), data.end());

    for (const auto &i:queue) {
        std::cout << i << '\n';
    }
}
