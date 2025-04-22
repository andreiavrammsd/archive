/*
 * https://www.think-cell.com/en/career/talks/pdf/think-cell_talk_memorymodel.pdf
 */

#include <atomic>
#include <cassert>

template<typename T>
class lock_free_list {
    struct node {
        explicit node(T d) : data{d} {}

        T data;
        node *next{};
    };

    std::atomic<node *> head{};
public:
    void push(T const &data) {
        node *const newNode = new node(data);
        newNode->next = head.load();
        while (!head.compare_exchange_weak(newNode->next, newNode));
    }

    T front() {
        return head.load()->data;
    }
};

int main() {
    lock_free_list<int> list;

    list.push(1);
    list.push(2);
    assert(list.front() == 2);
}
