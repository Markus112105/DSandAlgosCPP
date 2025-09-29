#include <iostream>
#include <queue>

// This program implements a basic Binary Search Tree (BST) that stores unique keys.
// Each node keeps pointers to its left and right children. The BST property guarantees the
// inorder traversal yields sorted values, which makes the tree handy for ordered data and
// fast (average-case O(log n)) lookup/insert/delete operations.
struct Node {
    int key;
    Node* left;
    Node* right;

    explicit Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        destroy(root);
    }

    // Insert refuses duplicates to keep the tree a strict set.
    void insert(int key) {
        root = insertRecursive(root, key);
    }

    bool contains(int key) const {
        return containsRecursive(root, key);
    }

    void remove(int key) {
        root = removeRecursive(root, key);
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
            std::cout << current->key << ' ';
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
            // Duplicate detected: ignore to preserve uniqueness.
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

    static Node* removeRecursive(Node* node, int key) {
        if (!node) {
            return nullptr;
        }
        if (key < node->key) {
            node->left = removeRecursive(node->left, key);
        } else if (key > node->key) {
            node->right = removeRecursive(node->right, key);
        } else {
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
            // Node with two children: swap with inorder successor (smallest in right subtree).
            Node* successor = findMin(node->right);
            node->key = successor->key;
            node->right = removeRecursive(node->right, successor->key);
        }
        return node;
    }

    static void printInOrderRecursive(Node* node) {
        if (!node) {
            return;
        }
        printInOrderRecursive(node->left);
        std::cout << node->key << ' ';
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
    BinarySearchTree tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    // Attempt to insert duplicate
    tree.insert(30);

    std::cout << "In-order traversal (sorted): ";
    tree.printInOrder();

    std::cout << "Level-order traversal: ";
    tree.printLevelOrder();

    std::cout << "Contains 40? " << (tree.contains(40) ? "Yes" : "No") << "\n";
    std::cout << "Contains 90? " << (tree.contains(90) ? "Yes" : "No") << "\n";

    tree.remove(50);
    std::cout << "In-order after removing 50: ";
    tree.printInOrder();

    return 0;
}
