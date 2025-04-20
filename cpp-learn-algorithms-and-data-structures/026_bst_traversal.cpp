#include <cassert>
#include <vector>
class BST {
   public:
    int value;
    BST *left{};
    BST *right{};

    BST(int value) : value{value} {}
};

// O(n) T | O(n) S
void inOrderTraverse(BST *tree, std::vector<int> &array) {
    if (tree == nullptr) {
        return;
    }

    inOrderTraverse(tree->left, array);
    array.push_back(tree->value);
    inOrderTraverse(tree->right, array);
}

// O(n) T | O(n) S
void preOrderTraverse(BST *tree, std::vector<int> &array) {
    if (tree == nullptr) {
        return;
    }

    array.push_back(tree->value);
    preOrderTraverse(tree->left, array);
    preOrderTraverse(tree->right, array);
}

// O(n) T | O(n) S
void postOrderTraverse(BST *tree, std::vector<int> &array) {
    if (tree == nullptr) {
        return;
    }

    postOrderTraverse(tree->left, array);
    postOrderTraverse(tree->right, array);
    array.push_back(tree->value);
}

int main() {
    auto c1 = new BST(1);

    auto b1 = new BST(2);
    b1->left = c1;

    auto b2 = new BST(5);
    auto b3 = new BST(22);

    auto a1 = new BST(5);
    a1->left = b1;
    a1->right = b2;

    auto a2 = new BST(15);
    a2->right = b3;

    auto root = new BST(10);
    root->left = a1;
    root->right = a2;

    std::vector<int> result;

    inOrderTraverse(root, result);
    assert(result == (std::vector<int>{1, 2, 5, 5, 10, 15, 22}));

    result.clear();
    preOrderTraverse(root, result);
    assert(result == (std::vector<int>{10, 5, 2, 1, 5, 15, 22}));

    result.clear();
    postOrderTraverse(root, result);
    assert(result == (std::vector<int>{1, 2, 5, 5, 22, 15, 10}));

    delete root;
    delete a1;
    delete a2;
    delete b1;
    delete b2;
    delete b3;
    delete c1;
}
