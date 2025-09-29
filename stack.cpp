#include <cstddef>
#include <iostream>
#include <stdexcept>

// This program implements a stack of integers backed by a dynamically resizing array.
// Push operations double the underlying storage when full so that amortized cost stays O(1),
// while pop and peek run in constant time with range checks that throw if the stack is empty.
class Stack {
public:
    explicit Stack(std::size_t initialCapacity = 8)
        : capacity(initialCapacity), topIndex(0) {
        if (capacity == 0) {
            capacity = 8;
        }
        // The underlying array acts as contiguous storage for the LIFO container.
        data = new int[capacity];
    }

    ~Stack() {
        // Destructor cleans up the heap allocation to prevent leaks.
        delete[] data;
    }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(int value) {
        if (topIndex == capacity) {
            // When storage is full, double the capacity so the amortized cost stays constant.
            grow();
        }
        // Place the new value at the current top and advance the top marker.
        data[topIndex++] = value;
    }

    int pop() {
        if (empty()) {
            throw std::out_of_range("Cannot pop from an empty stack");
        }
        // Decrement first so topIndex always points to the next open slot.
        return data[--topIndex];
    }

    int peek() const {
        if (empty()) {
            throw std::out_of_range("Cannot peek an empty stack");
        }
        // topIndex - 1 is the last pushed element because topIndex counts elements.
        return data[topIndex - 1];
    }

    bool empty() const {
        return topIndex == 0;
    }

    std::size_t size() const {
        return topIndex;
    }

private:
    void grow() {
        std::size_t newCapacity = capacity * 2;
        int* newData = new int[newCapacity];
        for (std::size_t i = 0; i < capacity; ++i) {
            // Copy existing elements to the new buffer; order must remain unchanged for stack semantics.
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    int* data = nullptr;
    // capacity tracks the size of the allocated array, topIndex counts active elements.
    std::size_t capacity = 0;
    std::size_t topIndex = 0;
};

int main() {
    Stack stack;

    for (int value : {3, 7, 11, 15}) {
        // Sequential pushes exercise automatic expansion once the default capacity is exceeded.
        stack.push(value);
    }

    std::cout << "Stack size after pushes: " << stack.size() << "\n";
    std::cout << "Top element is: " << stack.peek() << "\n";

    while (!stack.empty()) {
        // Pop until empty to prove the container shrinks logically even though capacity stays high.
        std::cout << "Popped " << stack.pop() << ", size now " << stack.size() << "\n";
    }

    try {
        stack.pop();
    } catch (const std::out_of_range& ex) {
        std::cout << "Exception caught: " << ex.what() << "\n";
    }

    return 0;
}
