#include <cassert>
#include <queue>
#include <string>
#include <vector>

// O(n+c) T | O(n) S
class Node {
   public:
    std::string name;
    std::vector<Node *> children;

    Node(std::string str) { name = str; }

    std::vector<std::string> breadthFirstSearch(std::vector<std::string> *array) {
        std::queue<Node *> nodes;
        nodes.push(this);

        while (!nodes.empty()) {
            Node *current = nodes.front();
            array->push_back(current->name);
            nodes.pop();

            for (auto child : current->children) {
                nodes.push(child);
            }
        }

        return *array;
    }

    Node *addChild(std::string name) {
        Node *child = new Node(name);
        children.push_back(child);
        return this;
    }
};

int main() {
    auto a = new Node("A");
    auto b = new Node("B");
    auto c = new Node("C");
    auto d = new Node("D");
    auto e = new Node("E");
    auto f = new Node("F");
    auto g = new Node("G");
    auto h = new Node("H");
    auto i = new Node("I");
    auto j = new Node("J");
    auto k = new Node("K");

    a->children = {b, c, d};
    b->children = {e, f};
    d->children = {g, h};
    f->children = {i, j};
    g->children = {k};

    std::vector<std::string> expected = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K"};
    std::vector<std::string> result;

    assert(a->breadthFirstSearch(&result) == expected);

    delete a;
    delete b;
    delete c;
    delete d;
    delete e;
    delete f;
    delete g;
    delete h;
    delete i;
    delete j;
    delete k;
}
