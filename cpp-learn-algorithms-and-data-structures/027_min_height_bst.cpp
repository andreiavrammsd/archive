#include <cassert>
#include <cmath>
#include <vector>
class BST {
   public:
    int value;
    BST *left;
    BST *right;

    BST(int value) {
        this->value = value;
        left = NULL;
        right = NULL;
    }

    void insert(int value) {
        if (value < this->value) {
            if (left == NULL) {
                left = new BST(value);
            } else {
                left->insert(value);
            }
        } else {
            if (right == NULL) {
                right = new BST(value);
            } else {
                right->insert(value);
            }
        }
    }
};

BST *minHeightBSSubtree(std::vector<int> array, int from, int to, BST *node) {
    if (to < from) {
        return nullptr;
    }

    int mid = (from + to) / 2;
    if (node == nullptr) {
        node = new BST(array[mid]);
    } else {
        node->insert(array[mid]);
    }

    if (mid == 0) {
        return node;
    }

    minHeightBSSubtree(array, from, mid - 1, node);
    minHeightBSSubtree(array, mid + 1, to, node);

    return node;
}

// O(nlog(n)) T | O(n) S
BST *minHeightBst(std::vector<int> array) { return minHeightBSSubtree(array, 0, array.size() - 1, nullptr); }

void preOrderTraverse(BST *tree, std::vector<int> &array) {
    if (tree == nullptr) {
        return;
    }

    array.push_back(tree->value);
    preOrderTraverse(tree->left, array);
    preOrderTraverse(tree->right, array);
}

BST *minHeightBSSubtree2(const std::vector<int> &array, int from, int to) {
    if (to < from) {
        return nullptr;
    }

    int mid = (from + to) / 2;

    BST *node = new BST(array[mid]);
    node->left = minHeightBSSubtree2(array, from, mid - 1);
    node->right = minHeightBSSubtree2(array, mid + 1, to);

    return node;
}

// O(n) ST
BST *minHeightBst2(const std::vector<int> &array) {
    return minHeightBSSubtree2(array, 0, static_cast<int>(array.size()) - 1);
}

int main() {
    auto input = std::vector<int>{1, 2, 5, 7, 10, 13, 14, 15, 22};
    auto expected = std::vector<int>{10, 2, 1, 5, 7, 14, 13, 15, 22};
    auto actual = std::vector<int>{};

    auto tree = minHeightBst(input);
    preOrderTraverse(tree, actual);
    assert(actual == expected);

    tree = minHeightBst2(input);
    actual.clear();
    preOrderTraverse(tree, actual);
    assert(actual == expected);
}
