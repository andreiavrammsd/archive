#include <algorithm>
#include <cassert>
#include <vector>
#include <limits>

class BST {
   public:
    int value;
    BST *left{};
    BST *right{};

    BST(int val) : value{val} {}
    BST &insert(int val);
};

// AVG O(log n) TS
int find(BST *tree, int target, int &closest_value, int distance) {
    if (tree == nullptr) {
        return closest_value;
    }

    auto dist = abs(tree->value - target);
    if (dist < distance) {
        closest_value = tree->value;
        distance = dist;
    }

    if (target <= tree->value) {
        find(tree->left, target, closest_value, distance);
    } else {
        find(tree->right, target, closest_value, distance);
    }

    return closest_value;
}

int findClosestValueInBst(BST *tree, int target) {
    int closest_value = 0;
    int distance = std::numeric_limits<int>::max();

    find(tree, target, closest_value, distance);

    return closest_value;
}

// AVG O(log n) T, O(1) S
int findClosestValueInBst2(BST *tree, int target) {
    int closest_value = -1;
    int distance = std::numeric_limits<int>::max();

    while (tree != nullptr) {
        auto dist = abs(target - tree->value);
        if (dist < distance) {
            distance = dist;
            closest_value = tree->value;
        }

        if (target <= tree->value) {
            tree = tree->left;
        } else {
            tree = tree->right;
        }
    }

    return closest_value;
}

// AVG O(log n) T, O(1) S
int findClosestValueInBst3(BST *tree, int target) {
    BST *current_node = tree;
    int closest_value = current_node->value;

    while (current_node != nullptr) {
        if (abs(target - current_node->value) < abs(target - closest_value)) {
            closest_value = current_node->value;
        }

        if (target < current_node->value) {
            current_node = current_node->left;
        } else if (target > current_node->value) {
            current_node = current_node->right;
        } else {
            break;
        }
    }

    return closest_value;
}

int main() {
    auto c1 = new BST(1001);
    auto b1 = new BST(2);

    auto b3 = new BST(204);
    auto b4 = new BST(55000);
    b4->left = c1;

    auto a1 = new BST(5);
    a1->left = b1;

    auto a2 = new BST(502);
    a2->left = b3;
    a2->right = b4;

    auto root = new BST(100);
    root->left = a1;
    root->right = a2;

    int target = 29751;

    assert(findClosestValueInBst(root, target) == 55000);
    assert(findClosestValueInBst2(root, target) == 55000);
    assert(findClosestValueInBst3(root, target) == 55000);

    delete root;
    delete a1;
    delete a2;
    delete b1;
    delete b3;
    delete b4;
    delete c1;
}
