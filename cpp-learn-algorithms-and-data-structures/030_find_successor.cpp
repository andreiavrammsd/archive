#include <cassert>
#include <vector>

class BinaryTree {
   public:
    int value;
    BinaryTree *left = nullptr;
    BinaryTree *right = nullptr;
    BinaryTree *parent = nullptr;

    BinaryTree(int value) { this->value = value; }
};

void getNodes(BinaryTree *node, std::vector<BinaryTree *> &nodes) {
    if (node == nullptr) {
        return;
    }

    getNodes(node->left, nodes);
    nodes.push_back(node);
    getNodes(node->right, nodes);
}

// O(n) TS
BinaryTree *findSuccessor(BinaryTree *tree, BinaryTree *node) {
    std::vector<BinaryTree *> nodes;
    getNodes(tree, nodes);

    for (size_t i = 0; i < nodes.size() - 1; ++i) {
        if (nodes[i]->value == node->value) {
            return nodes[i + 1];
        }
    }

    return nullptr;
}

BinaryTree *findSuccessorNode(BinaryTree *tree, BinaryTree *node, bool &found) {
    if (tree == nullptr) {
        return nullptr;
    }

    auto left = findSuccessorNode(tree->left, node, found);
    if (left != nullptr) {
        return left;
    }

    if (found) {
        return tree;
    }
    if (tree == node) {
        found = true;
    }

    auto right = findSuccessorNode(tree->right, node, found);
    if (right != nullptr) {
        return right;
    }

    return nullptr;
}

// O(n) T | O(h) S
BinaryTree *findSuccessor2(BinaryTree *tree, BinaryTree *node) {
    bool found = false;
    return findSuccessorNode(tree, node, found);
}

// O(n) T | O(h) S
BinaryTree *findSuccessor3(BinaryTree *tree, BinaryTree *node) {
    if (tree == nullptr) {
        return nullptr;
    }

    if (node->right != nullptr) {
        BinaryTree *current = node->right;
        while (current != nullptr) {
            if (current->left == nullptr) {
                return current;
            }
            current = current->left;
        }
    }

    BinaryTree *current = node;
    BinaryTree *parent = current->parent;
    while (parent != nullptr) {
        if (parent->left == current) {
            return parent;
        }

        current = parent;
        parent = parent->parent;
    }

    return nullptr;
}

int main() {
    // #1
    auto c1 = new BinaryTree(6);

    auto b1 = new BinaryTree(4);
    b1->left = c1;

    auto b2 = new BinaryTree(5);

    auto a1 = new BinaryTree(2);
    a1->left = b1;
    a1->right = b2;

    auto a2 = new BinaryTree(3);

    auto root = new BinaryTree(1);
    root->left = a1;
    root->right = a2;

    a1->parent = root;
    a2->parent = root;
    b1->parent = a1;
    b2->parent = a1;
    c1->parent = b1;

    assert(findSuccessor(root, b2) == root);
    assert(findSuccessor2(root, b2) == root);
    assert(findSuccessor3(root, b2) == root);

    delete root;
    delete a1;
    delete a2;
    delete b1;
    delete b2;
    delete c1;

    // #2
    auto d1 = new BinaryTree(5);

    c1 = new BinaryTree(4);
    c1->left = d1;

    b1 = new BinaryTree(3);
    b1->left = c1;

    a1 = new BinaryTree(2);
    a1->left = b1;

    root = new BinaryTree(1);
    root->left = a1;

    a1->parent = root;
    b1->parent = a1;
    c1->parent = b1;
    d1->parent = c1;

    assert(findSuccessor(root, b1) == a1);
    assert(findSuccessor2(root, b1) == a1);
    assert(findSuccessor3(root, b1) == a1);

    delete root;
    delete a1;
    delete b1;
    delete c1;
    delete d1;
}
