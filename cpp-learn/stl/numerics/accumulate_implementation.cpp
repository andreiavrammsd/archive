/*
 The C++ Programming Language, 4th Edition
 */

#include <functional>
#include <vector>
#include <cassert>

template<typename T>
struct custom_operation {
    T operator()(T acc, T n) {
        return acc + n * 2;
    }
};

template<typename Iter, typename Val, typename Oper = std::plus<>>
Val accumulate(Iter first, Iter last, Val s, Oper op = std::plus<>{}) {
    while (first != last) {
        s = op(s, *first);
        ++first;
    }
    return s;
}

struct Node {
    Node *next{};
    int data{};
};

int operator+(int s, Node n) {
    if (n.next == nullptr) {
        return s;
    }
    return s + n.data;
}

int main() {
    // Array
    double ad[] = {1, 2, 3, 4};

    double s1 = accumulate(ad, ad + 4, 0, std::plus<>{});
    assert(s1 == 10);

    double s2 = accumulate(ad, ad + 4, 1, std::multiplies<>{});
    assert(s2 == 24);

    // Vector
    std::vector<int> v{1, 2, 3, 4};
    int s3 = accumulate(v.begin(), v.end(), 0, std::plus<>{});
    assert(s3 == 10);

    int s4 = accumulate(v.begin(), v.end(), 0, custom_operation<int>{});
    assert(s4 == 20);

    // User-defined
    auto n1 = new Node{};
    n1->data = 1;

    auto n2 = new Node{};
    n2->data = 2;

    auto n3 = new Node{};
    n3->data = 3;

    auto n4 = new Node{};
    n4->data = 4;

    auto n5 = new Node{};

    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n5;

    int s5 = accumulate(n1, n5, 0);
    assert(s5 == 10);
}
