#include <cassert>
#include <iostream>
#include <vector>

class BinaryTree {
   public:
    int value;
    BinaryTree *left;
    BinaryTree *right;

    BinaryTree(int value) {
        this->value = value;
        left = NULL;
        right = NULL;
    }
};

void computeSums(BinaryTree *node, std::vector<int> &sums, int sum) {
    if (node == nullptr) {
        return;
    }

    sum += node->value;

    if (node->left != nullptr) {
        computeSums(node->left, sums, sum);
    }

    if (node->right != nullptr) {
        computeSums(node->right, sums, sum);
    }

    if (node->left == nullptr && node->right == nullptr) {
        sums.push_back(sum);
    }
}

std::vector<int> branchSums(BinaryTree *root) {
    std::vector<int> sums;
    int sum = 0;
    computeSums(root, sums, sum);

    return sums;
}

int main() {
    auto c1 = new BinaryTree(8);
    auto c2 = new BinaryTree(9);
    auto c3 = new BinaryTree(10);

    auto b1 = new BinaryTree(4);
    b1->left = c1;
    b1->right = c2;

    auto b2 = new BinaryTree(5);
    b2->left = c3;

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

    assert(branchSums(root) == (std::vector<int>{15, 16, 18, 10, 11}));

    delete root;
    delete a1;
    delete a2;
    delete b1;
    delete b2;
    delete b3;
    delete b4;
    delete c1;
    delete c2;
    delete c3;
}
