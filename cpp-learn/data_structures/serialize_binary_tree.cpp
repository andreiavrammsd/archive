#include <string>
#include <iostream>
#include <utility>
#include <cassert>
#include <sstream>

template<typename T>
class Node {
public:
    explicit Node(T val, const Node *left = nullptr, const Node *right = nullptr) : val(std::move(val)) {
        this->left = left;
        this->right = right;
    }

    virtual ~Node() {
        delete left;
        delete right;
    }

    const T val;
    const Node *left;
    const Node *right;
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const Node<T> *node) {
    os << serialize(node);
    return os;
}

template<typename T>
std::string serialize(const Node<T> *node) {
    if (node == nullptr) {
        return "";
    }

    std::stringstream serialized;
    serialized << node->val << "|";
    serialized << serialize(node->left);
    serialized << serialize(node->right);

    return serialized.str();
}

int main() {
    auto right = new Node<std::string>("right");
    auto leftLeft = new Node<std::string>("left.left");
    auto left = new Node<std::string>("left", leftLeft);
    auto root = new Node<std::string>("root", left, right);

    assert(serialize(root) == "root|left|left.left|right|");
    std::cout << root << std::endl;
    delete root;

    auto n = new Node<int>(1);
    auto m = new Node<int>(0, n);

    assert(serialize(m) == "0|1|");
    std::cout << m << std::endl;
    delete m;
}
