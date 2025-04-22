#include <vector>
#include <cassert>
#include <memory>

template<typename T>
struct LinkedList {
private:
    struct Node {
        T value{};
        std::shared_ptr<Node> next{};
    };
public:
    std::shared_ptr<Node> head{};

    void push(T value) {
        auto node = std::make_shared<Node>();
        node->value = value;
        node->next = head;
        head = node;
    }

    void push_back(T value) {
        auto node = std::make_shared<Node>();
        node->value = value;

        auto last = head;
        while (last->next) {
            last = last->next;
        }

        last->next = node;
    }

    void remove(T value) {
        auto current = head;
        auto prev = head;

        while (current) {
            if (current->value == value) {
                prev->next = current->next;
            } else {
                prev = current;
            }

            current = current->next;
        }
    }
};

template<typename T>
std::vector<T> values(const LinkedList<T> &list) {
    std::vector<T> values{};

    auto node = list.head;
    while (node) {
        values.push_back(node->value);
        node = node->next;
    }

    return values;
}

int main() {
    LinkedList<int> list;

    list.push(1);
    list.push(2);
    list.push(2);
    list.push(3);
    list.push(4);
    list.push(4);
    list.push(5);
    assert(values(list) == (std::vector<int>{5, 4, 4, 3, 2, 2, 1}));

    list.remove(4);
    assert(values(list) == (std::vector<int>{5, 3, 2, 2, 1}));

    list.remove(1);
    assert(values(list) == (std::vector<int>{5, 3, 2, 2}));

    list.push_back(0);
    assert(values(list) == (std::vector<int>{5, 3, 2, 2, 0}));
}
