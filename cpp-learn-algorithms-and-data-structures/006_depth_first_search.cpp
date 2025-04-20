#include <cassert>
#include <string>
#include <vector>

class Node {
   public:
    std::string name;
    std::vector<Node *> children;

    Node(std::string str) { name = str; }

    // O(v + e) T, O(v) S
    std::vector<std::string> depthFirstSearch(std::vector<std::string> *array) {
        if (array == nullptr) {
            return {};
        }

        array->push_back(name);

        for (auto child : children) {
            child->depthFirstSearch(array);
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
    auto K = new Node("K");
    auto J = new Node("J");
    auto I = new Node("I");
    auto H = new Node("H");

    auto G = new Node("G");
    G->children = {K};

    auto F = new Node("F");
    F->children = {I, J};

    auto E = new Node("E");

    auto D = new Node("D");
    D->children = {G, H};

    auto C = new Node("C");

    auto B = new Node("B");
    B->children = {E, F};

    auto A = new Node("A");
    A->children = {B, C, D};

    std::vector<std::string> names;
    auto expected = std::vector<std::string>{"A", "B", "E", "F", "I", "J", "C", "D", "G", "K", "H"};
    assert(A->depthFirstSearch(&names) == expected);

    delete A;
    delete B;
    delete C;
    delete D;
    delete E;
    delete F;
    delete G;
    delete H;
    delete I;
    delete J;
    delete K;
}
