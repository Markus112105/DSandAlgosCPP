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
        Node* node = new Node(value);
        node->next = head;
        head = node;
        if (!tail) {
            tail = node;
        }
        ++length;
    }

    void pushBack(int value) {
        Node* node = new Node(value);
        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        ++length;
    }

    bool insertAfter(int target, int value) {
        Node* current = head;
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
            tail = node;
        }
        ++length;
        return true;
    }

    bool remove(int value) {
        Node* previous = nullptr;
        Node* current = head;
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
            head = current->next;
        }
        if (current == tail) {
            tail = previous;
        }
        delete current;
        --length;
        if (length == 0) {
            tail = nullptr;
        }
        return true;
    }

    bool contains(int value) const {
        Node* current = head;
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

