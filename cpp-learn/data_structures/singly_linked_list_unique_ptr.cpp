#include <vector>
#include <cassert>
#include <memory>

template<typename T>
struct LinkedList {
private:
    struct Node {
        T value{};
        std::unique_ptr<Node> next{};
    };

public:
    std::unique_ptr<Node> head{};

    void push(T value) {
        auto node = std::make_unique<Node>();
        node->value = value;
        node->next = move(head);
        head = move(node);
    }

    T front() {
        return head->value;
    }

    void pop() {
        head = move(head->next);
    }
};

int main() {
    LinkedList<int> list;

    list.push(1);
    list.push(2);
    list.push(3);
    list.push(4);

    assert(list.front() == 4);

    list.pop();
    assert(list.front() == 3);

    list.pop();
    assert(list.front() == 2);

    list.push(1);
    assert(list.front() == 1);
}
