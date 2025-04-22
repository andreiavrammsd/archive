/*
 The C++ Programming Language, 4th Edition
 */

#include <vector> // One-dimensional resizable array
#include <deque> // Double-ended queue
#include <forward_list> // Singly-linked list
#include <list> // Doubly-linked list
#include <map> // Associative array
#include <set> // Set
#include <unordered_map> // Hashed associative array
#include <unordered_set> // Hashed set
#include <queue> // Queue
#include <stack> // Stack
#include <array> // One-dimensional fixed-size array
#include <bitset> // Array of bool

#include <stdexcept>
#include <cassert>

int main() {
    // One-dimensional resizable array
    std::vector<int> vector(5);
    assert(vector.size() == 5);
    assert(vector.capacity() == 5);
    try {
        vector.at(6);
    } catch (const std::out_of_range &) {}

    // Double-ended queue
    std::deque<int> dq;
    dq.emplace_back(1);
    assert(dq.front() == 1);

    // Singly-linked list
    std::forward_list<int> fl;
    fl.emplace_front(1);
    fl.emplace_front(2);
    fl.emplace_after(fl.begin(), 3);
    assert(fl.front() == 2);
    fl.pop_front();
    assert(fl.front() == 3);

    // Doubly-linked list
    std::list<int> list{2};
    assert(list.size() == 1);

    // Associative array
    std::map<int, int> map;
    map[1] = 2;
    map.clear();
    assert(map.empty());

    std::multimap<int, int> multimap;
    multimap.emplace(2, 0);
    multimap.emplace(2, 1);
    auto it = multimap.begin();
    assert(it->first == 2);
    assert(it->second == 0);
    ++it;
    assert(it->first == 2);
    assert(it->second == 1);

    // Set
    std::set<int> set;
    set.insert(2);
    set.insert(2);
    assert(set.size() == 1);

    std::multiset<int> multiset;
    multiset.insert(1);
    multiset.insert(1);
    assert(multiset.size() == 2);

    // Hashed associative array
    std::unordered_map<int, int> um;
    std::pair<int, int> pair{1, 2};
    um.insert(pair);
    assert(um[1] == 2);
    um[1] = 3;
    assert(um[1] == 3);

    // Hashed set
    std::unordered_set<int> us;
    us.insert(1);
    us.insert(1);
    assert(us.size() == 1);

    // Queue
    std::queue<int> q;
    q.push(2);
    q.push(1);
    assert(q.front() == 2);

    std::priority_queue<int> pq;
    pq.push(3);
    pq.push(10);
    assert(pq.top() == 10);

    // Stack
    std::stack<int> stack;
    stack.push(1);
    stack.push(2);
    assert(stack.top() == 2);

    // One-dimensional fixed-size array
    std::array<int, 2> array{3, 6};
    static_assert(array.size() == 2);
    assert(array.back() == 6);

    // Array of bool
    std::bitset<3> bs;
    static_assert(bs.size() == 3);
    bs.flip();
    assert(bs.all());
}
