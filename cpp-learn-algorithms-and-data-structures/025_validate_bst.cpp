#include <cassert>
#include <limits>
class BST {
   public:
    int value;
    BST *left{};
    BST *right{};

    BST(int val) : value{val} {}
    BST &insert(int val);
};

bool validate(BST *node, int minValue, int maxValue) {
    if (node == nullptr) {
        return true;
    }

    if (node->value >= minValue && node->value < maxValue) {
        return validate(node->left, minValue, node->value) && validate(node->right, node->value, maxValue);
    }

    return false;
}

// O(n) T, O(d) S
bool validateBst(BST *tree) { return validate(tree, std::numeric_limits<int>::min(), std::numeric_limits<int>::max()); }

int main() {
    auto b2 = new BST(6);
    auto b3 = new BST(16);

    auto a1 = new BST(5);
    a1->right = b2;

    auto a2 = new BST(15);
    a2->right = b3;

    auto root = new BST(10);
    root->left = a1;
    root->right = a2;

    assert(validateBst(root));

    delete root;
    delete a1;
    delete a2;
    delete b2;
    delete b3;
}
