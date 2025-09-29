#include <iostream>
#include <queue>

// This program implements a Binary Search Tree variant that admits duplicate keys.
// Rather than storing duplicates in separate nodes, each tree node tracks a frequency count.
// This approach keeps the structure height-sensitive like a standard BST while maintaining
// accurate multiplicity information for multiset-style workloads.
struct Node {
    int key;
    int count;
    Node* left;
    Node* right;

    explicit Node(int k)
        : key(k), count(1), left(nullptr), right(nullptr) {}
};

class MultisetBST {
public:
    MultisetBST() : root(nullptr) {}

    ~MultisetBST() {
        destroy(root);
    }

    void insert(int key) {
        root = insertRecursive(root, key);
    }

    bool contains(int key) const {
        return containsRecursive(root, key);
    }

    void eraseOne(int key) {
        root = eraseRecursive(root, key, false);
    }

    void eraseAll(int key) {
        root = eraseRecursive(root, key, true);
    }

    void printInOrder() const {
        printInOrderRecursive(root);
        std::cout << "\n";
    }

    void printLevelOrder() const {
        if (!root) {
            return;
        }
        std::queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            std::cout << current->key << "(x" << current->count << ") ";
            if (current->left) {
                q.push(current->left);
            }
            if (current->right) {
                q.push(current->right);
            }
        }
        std::cout << "\n";
    }

private:
    Node* root;

    static Node* insertRecursive(Node* node, int key) {
        if (!node) {
            return new Node(key);
        }
        if (key < node->key) {
            node->left = insertRecursive(node->left, key);
        } else if (key > node->key) {
            node->right = insertRecursive(node->right, key);
        } else {
            ++node->count;
        }
        return node;
    }

    static bool containsRecursive(Node* node, int key) {
        if (!node) {
            return false;
        }
        if (key < node->key) {
            return containsRecursive(node->left, key);
        }
        if (key > node->key) {
            return containsRecursive(node->right, key);
        }
        return true;
    }

    static Node* findMin(Node* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    static Node* eraseRecursive(Node* node, int key, bool removeAll) {
        if (!node) {
            return nullptr;
        }
        if (key < node->key) {
            node->left = eraseRecursive(node->left, key, removeAll);
        } else if (key > node->key) {
            node->right = eraseRecursive(node->right, key, removeAll);
        } else {
            if (!removeAll && node->count > 1) {
                --node->count;
                return node;
            }
            if (!node->left) {
                Node* rightChild = node->right;
                delete node;
                return rightChild;
            }
            if (!node->right) {
                Node* leftChild = node->left;
                delete node;
                return leftChild;
            }
            // Replace with inorder successor and peel off one occurrence there.
            Node* successor = findMin(node->right);
            node->key = successor->key;
            node->count = successor->count;
            successor->count = 1;
            node->right = eraseRecursive(node->right, successor->key, true);
        }
        return node;
    }

    static void printInOrderRecursive(Node* node) {
        if (!node) {
            return;
        }
        printInOrderRecursive(node->left);
        for (int i = 0; i < node->count; ++i) {
            std::cout << node->key << ' ';
        }
        printInOrderRecursive(node->right);
    }

    static void destroy(Node* node) {
        if (!node) {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
};

int main() {
    MultisetBST multisetTree;
    multisetTree.insert(10);
    multisetTree.insert(5);
    multisetTree.insert(15);
    multisetTree.insert(10);
    multisetTree.insert(10);
    multisetTree.insert(5);

    std::cout << "In-order traversal (duplicates repeated): ";
    multisetTree.printInOrder();

    std::cout << "Level-order traversal with counts: ";
    multisetTree.printLevelOrder();

    std::cout << "Contains 15? " << (multisetTree.contains(15) ? "Yes" : "No") << "\n";
    std::cout << "Contains 20? " << (multisetTree.contains(20) ? "Yes" : "No") << "\n";

    multisetTree.eraseOne(10);
    std::cout << "After erasing one 10: ";
    multisetTree.printInOrder();

    multisetTree.eraseAll(5);
    std::cout << "After erasing all 5s: ";
    multisetTree.printInOrder();

    return 0;
}
