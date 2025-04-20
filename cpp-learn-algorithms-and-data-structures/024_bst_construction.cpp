#include <cassert>
#include <iostream>
#include <vector>

class BST {
   public:
    int value;
    BST *left;
    BST *right;

    BST(int val) {
        value = val;
        left = NULL;
        right = NULL;
    }

    // avg: O(log n) T | O(1) S
    // worst: O(n) T | O(1) S
    BST &insert(int val) {
        BST *current = this;

        while (current != nullptr) {
            if (val < current->value) {
                if (current->left == nullptr) {
                    current->left = new BST(val);
                    return *this;
                }

                current = current->left;
            } else {
                if (current->right == nullptr) {
                    current->right = new BST(val);
                    return *this;
                }

                current = current->right;
            }
        }
        return *this;
    }

    // avg: O(log n) T | O(1) S
    // worst: O(n) T | O(1) S
    bool contains(int val) {
        BST *current = this;

        while (current != nullptr) {
            if (current->value == val) {
                return true;
            }

            if (val < current->value) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        return false;
    }

    // avg: O(log n) T | O(1) S
    // worst: O(n) T | O(1) S
    BST &remove(int val) {
        BST *current = this;
        BST *prev = nullptr;

        while (current != nullptr) {
            if (current->value == val) {
                if (current->left == nullptr && current->right == nullptr) {
                    if (prev == nullptr) {
                        return *this;
                    }

                    if (val < prev->value) {
                        prev->left = nullptr;
                    } else {
                        prev->right = nullptr;
                    }

                    return *this;
                }

                if (current->left != nullptr && current->right == nullptr) {
                    if (prev == nullptr) {  // is at root
                        current->value = current->left->value;
                        current->left = current->left->left;
                    } else {
                        prev->left = current->left;
                    }

                    return *this;
                }

                if (current->left == nullptr && current->right != nullptr) {
                    if (prev == nullptr) {  // is at root
                        current->value = current->right->value;
                        current->right = current->right->right;
                    } else {
                        if (val < prev->value) {
                            prev->left = current->right;
                        } else {
                            prev->right = current->right;
                        }
                    }

                    return *this;
                }

                BST *minValueNodeParent = findMinValueNodeParent(current->right);

                if (minValueNodeParent != nullptr) {
                    current->value = minValueNodeParent->left->value;
                    minValueNodeParent->left = nullptr;
                } else {
                    current->value = current->right->value;
                    current->right = nullptr;
                }

                return *this;
            }

            if (val < current->value) {
                prev = current;
                current = current->left;
            } else {
                prev = current;
                current = current->right;
            }
        }

        return *this;
    }

    static BST *findMinValueNodeParent(BST *node) {
        BST *current = node;
        BST *prev = nullptr;
        while (current->left != nullptr) {
            prev = current;
            current = current->left;
        }

        return prev;
    }
};

void print(BST *node) {
    if (node == nullptr) {
        return;
    }

    std::cout << node->value << ", ";

    print(node->left);
    print(node->right);
}

void list(BST *node, std::vector<int> &elements) {
    if (node == nullptr) {
        return;
    }

    elements.push_back(node->value);

    list(node->left, elements);
    list(node->right, elements);
}

int main() {
    auto c1 = new BST(1);
    auto c2 = new BST(14);

    auto b1 = new BST(2);
    b1->left = c1;

    auto b2 = new BST(5);

    auto b3 = new BST(13);
    b3->right = c2;

    auto b4 = new BST(22);

    auto a1 = new BST(5);
    a1->left = b1;
    a1->right = b2;

    auto a2 = new BST(15);
    a2->left = b3;
    a2->right = b4;

    auto root = new BST(10);
    root->left = a1;
    root->right = a2;

    assert(root->contains(15));
    assert(!root->contains(16));

    root->insert(16);
    assert(root->contains(16));

    std::vector<int> actual;
    std::vector<int> expected;

    root->insert(12);

    // List all
    actual.clear();
    expected = {10, 5, 2, 1, 5, 15, 13, 12, 14, 22, 16};
    list(root, actual);
    assert(actual == expected);
    print(root);
    std::cout << "\n";

    // Remove leaf from left
    root->remove(16);
    actual.clear();
    expected = {10, 5, 2, 1, 5, 15, 13, 12, 14, 22};
    list(root, actual);
    assert(actual == expected);
    print(root);
    std::cout << "\n";

    // Remove leaf from right
    root->remove(14);
    actual.clear();
    expected = {10, 5, 2, 1, 5, 15, 13, 12, 22};
    list(root, actual);
    assert(actual == expected);
    print(root);
    std::cout << "\n";

    // Remove parent with single child on left
    root->remove(13);
    actual.clear();
    expected = {10, 5, 2, 1, 5, 15, 12, 22};
    list(root, actual);
    assert(actual == expected);
    print(root);
    std::cout << "\n";

    // Remove parent with single child on right
    root->insert(13);
    root->remove(12);
    actual.clear();
    expected = {10, 5, 2, 1, 5, 15, 13, 22};
    list(root, actual);
    assert(actual == expected);
    print(root);
    std::cout << "\n";

    // Remove parent with two children
    root->remove(10);
    actual.clear();
    expected = {13, 5, 2, 1, 5, 15, 22};
    list(root, actual);
    assert(actual == expected);
    print(root);
    std::cout << "\n";
}
