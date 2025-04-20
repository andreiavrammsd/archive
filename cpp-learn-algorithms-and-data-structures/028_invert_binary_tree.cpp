#include <cassert>
#include <stack>
#include <vector>
class BinaryTree {
   public:
    int value;
    BinaryTree *left{};
    BinaryTree *right{};

    BinaryTree(int value) : value{value} {};
    void insert(std::vector<int> values, int i = 0);
    void invertedInsert(std::vector<int> values, int i = 0);
};

// O(n) T | O(d) S
void invertBinaryTree(BinaryTree *tree) {
    if (tree == nullptr) {
        return;
    }

    if (tree->left == nullptr && tree->right == nullptr) {
        return;
    }

    invertBinaryTree(tree->left);
    invertBinaryTree(tree->right);
    std::swap(tree->left, tree->right);
}

// O(n) TS
void invertBinaryTree2(BinaryTree *tree) {
    if (tree == nullptr) {
        return;
    }

    BinaryTree *node = tree;

    std::stack<BinaryTree *> nodes;
    nodes.push(node);

    while (!nodes.empty()) {
        node = nodes.top();
        std::swap(node->left, node->right);
        nodes.pop();

        if (node->left != nullptr) {
            nodes.push(node->left);
        }

        if (node->right) {
            nodes.push(node->right);
        }
    }
}

void preOrderTraverse(BinaryTree *tree, std::vector<int> &array) {
    if (tree == nullptr) {
        return;
    }

    array.push_back(tree->value);
    preOrderTraverse(tree->left, array);
    preOrderTraverse(tree->right, array);
}

int main() {
    auto c1 = new BinaryTree(8);
    auto c2 = new BinaryTree(9);

    auto b1 = new BinaryTree(4);
    b1->left = c1;
    b1->right = c2;

    auto b2 = new BinaryTree(5);
    auto b3 = new BinaryTree(6);
    auto b4 = new BinaryTree(7);

    auto a1 = new BinaryTree(2);
    a1->left = b1;
    a1->right = b2;

    auto a2 = new BinaryTree(3);
    a2->left = b3;
    a2->right = b4;

    auto root = new BinaryTree(1);
    root->left = a1;
    root->right = a2;

    auto actual = std::vector<int>{};

    invertBinaryTree(root);
    preOrderTraverse(root, actual);
    assert(actual == (std::vector<int>{1, 3, 7, 6, 2, 5, 4, 9, 8}));

    invertBinaryTree2(root);
    actual.clear();
    preOrderTraverse(root, actual);
    assert(actual == (std::vector<int>{1, 2, 4, 8, 9, 5, 3, 6, 7}));
}
