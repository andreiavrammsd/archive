#include <cassert>
#include <tuple>
#include <algorithm>

class BinaryTree {
   public:
    int value;
    BinaryTree *left;
    BinaryTree *right;

    BinaryTree(int value) {
        this->value = value;
        left = nullptr;
        right = nullptr;
    }
};

// O(n) TS
std::pair<int, int> findTreeData(BinaryTree *node) {
    if (node == nullptr) {
        return {0, 0};
    }

    auto leftData = findTreeData(node->left);
    auto rightData = findTreeData(node->right);

    auto maxDiameter = std::max(leftData.first, rightData.first);
    auto longestPath = leftData.second + rightData.second;
    auto diameter = std::max(maxDiameter, longestPath);
    auto height = std::max(leftData.second, rightData.second) + 1;

    return {diameter, height};
}

int binaryTreeDiameter(BinaryTree *tree) { return findTreeData(tree).first; }

int main() {
    auto d1 = new BinaryTree(9);
    auto d2 = new BinaryTree(6);

    auto c1 = new BinaryTree(8);
    c1->left = d1;

    auto c2 = new BinaryTree(5);
    c2->left = d2;

    auto b1 = new BinaryTree(7);
    b1->left = c1;

    auto b2 = new BinaryTree(4);
    b2->right = c2;

    auto b3 = new BinaryTree(6);
    auto b4 = new BinaryTree(7);

    auto a1 = new BinaryTree(3);
    a1->left = b1;
    a1->right = b2;

    auto a2 = new BinaryTree(2);
    a2->left = b3;
    a2->right = b4;

    auto root = new BinaryTree(1);
    root->left = a1;
    root->right = a2;

    assert(binaryTreeDiameter(root) == 6);
}
