#include <cassert>
#include <stack>
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

// Avg when tree is balanced: O(n) T, O(h) S
void sumDepths(BinaryTree *node, int &sum, int depth) {
    if (node == nullptr) {
        return;
    }

    sum += depth;

    ++depth;
    sumDepths(node->left, sum, depth);
    sumDepths(node->right, sum, depth);
}

int nodeDepths(BinaryTree *root) {
    int sum = 0;
    int depth = 0;
    sumDepths(root, sum, depth);
    return sum;
}

// Avg when tree is balanced: O(n) T, O(h) S
int nodeDepths2(BinaryTree *root, int depth = 0) {
    if (root == nullptr) {
        return 0;
    }

    return depth + nodeDepths2(root->left, depth + 1) + nodeDepths2(root->right, depth + 1);
}

// Avg when tree is balanced: O(n) T, O(h) S
int nodeDepths3(BinaryTree *root) {
    int sum = 0;

    auto nodes = std::stack<std::pair<BinaryTree *, int>>{};
    nodes.push({root, 0});

    while (!nodes.empty()) {
        auto nodeInfo = nodes.top();
        nodes.pop();

        if (nodeInfo.first == nullptr) {
            continue;
        }

        sum += nodeInfo.second;
        nodes.push({nodeInfo.first->left, nodeInfo.second + 1});
        nodes.push({nodeInfo.first->right, nodeInfo.second + 1});
    }

    return sum;
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

    assert(nodeDepths(root) == 16);
    assert(nodeDepths2(root) == 16);
    assert(nodeDepths3(root) == 16);

    delete root;
    delete a1;
    delete a2;
    delete b1;
    delete b2;
    delete b3;
    delete b4;
    delete c1;
    delete c2;
}
