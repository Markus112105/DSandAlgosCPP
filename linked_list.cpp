#include <cstddef>
#include <iostream>

// This program implements a singly linked list that stores integers while supporting
// fast insertions at both the head and tail, targeted insertion after a value, and
// removal by value. The list maintains head and tail pointers so append operations
// stay O(1), and it tracks its size for quick length queries.

class SinglyLinkedList {
public:
    ~SinglyLinkedList() {
        clear();
    }

    void pushFront(int value) {
        // Prepend by wiring the new node to the existing head and updating head pointer.
        Node* node = new Node(value);
        node->next = head;
        head = node;
        if (!tail) {
            // When inserting into an empty list, head and tail both reference the new node.
            tail = node;
        }
        ++length;
    }

    void pushBack(int value) {
        Node* node = new Node(value);
        if (!head) {
            head = tail = node;
        } else {
            // Append by linking the prior tail to the new node and sliding the tail pointer forward.
            tail->next = node;
            tail = node;
        }
        ++length;
    }

    bool insertAfter(int target, int value) {
        Node* current = head;
        // Linear scan locates the first node with the requested target value.
        while (current && current->value != target) {
            current = current->next;
        }
        if (!current) {
            return false;
        }
        Node* node = new Node(value);
        node->next = current->next;
        current->next = node;
        if (current == tail) {
            // Inserting after the tail requires updating tail so future appends stay O(1).
            tail = node;
        }
        ++length;
        return true;
    }

    bool remove(int value) {
        Node* previous = nullptr;
        Node* current = head;
        // Find the first node matching the requested value while retaining the predecessor pointer.
        while (current && current->value != value) {
            previous = current;
            current = current->next;
        }
        if (!current) {
            return false;
        }
        if (previous) {
            previous->next = current->next;
        } else {
            // Deleting the head shifts the head pointer forward one node.
            head = current->next;
        }
        if (current == tail) {
            // Removing the tail requires backing the tail pointer up to the previous node.
            tail = previous;
        }
        delete current;
        --length;
        if (length == 0) {
            // Keep tail consistent when the list becomes empty.
            tail = nullptr;
        }
        return true;
    }

    bool contains(int value) const {
        Node* current = head;
        // Standard pointer walk: return true on the first match, false if we fall off the list.
        while (current) {
            if (current->value == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    std::size_t size() const {
        return length;
    }

    bool empty() const {
        return length == 0;
    }

    void clear() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            // Delete nodes in sequence to release memory allocated via new.
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    void print(std::ostream& os) const {
        os << '[';
        Node* current = head;
        while (current) {
            os << current->value;
            if (current->next) {
                os << " -> ";
            }
            current = current->next;
        }
        os << ']';
    }

private:
    struct Node {
        // Every node holds one integer payload and a pointer to the next node in the chain.
        explicit Node(int v) : value(v), next(nullptr) {}
        int value;
        Node* next;
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    std::size_t length = 0;
};

int main() {
    SinglyLinkedList list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushFront(5);

    // Each print displays the linkage order so we can visualize how operations reshape the list.
    std::cout << "After pushFront/pushBack: ";
    list.print(std::cout);
    std::cout << " (size = " << list.size() << ")\n";

    list.insertAfter(10, 15);
    list.insertAfter(20, 25);
    std::cout << "After insertAfter calls: ";
    list.print(std::cout);
    std::cout << "\n";

    list.remove(5);
    list.remove(25);
    std::cout << "After remove calls: ";
    list.print(std::cout);
    std::cout << "\n";

    std::cout << "List contains 15? " << (list.contains(15) ? "yes" : "no") << "\n";
    std::cout << "List contains 42? " << (list.contains(42) ? "yes" : "no") << "\n";
    std::cout << "List is empty? " << (list.empty() ? "yes" : "no") << "\n";

    list.clear();
    std::cout << "After clear the list is empty? " << (list.empty() ? "yes" : "no") << "\n";

    return 0;
}
