#include <cstddef>
#include <iostream>
#include <stdexcept>

// This program implements a FIFO queue of integers backed by a circular buffer that grows
// dynamically when full. Enqueue operations run in amortized constant time, while dequeue
// and front checks validate the queue is not empty before returning the next value.
class Queue {
public:
    explicit Queue(std::size_t initialCapacity = 8)
        : capacity(initialCapacity), headIndex(0), tailIndex(0), count(0) {
        if (capacity == 0) {
            capacity = 8;
        }
        // Allocate the initial ring buffer; headIndex/tailIndex will move modulo capacity.
        data = new int[capacity];
    }

    ~Queue() {
        // Release the backing array on destruction.
        delete[] data;
    }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void enqueue(int value) {
        if (count == capacity) {
            // When the ring is saturated, double it so the amortized enqueue cost stays O(1).
            grow();
        }
        // Write at tailIndex, then advance tailIndex circularly so the structure wraps around.
        data[tailIndex] = value;
        tailIndex = (tailIndex + 1) % capacity;
        ++count;
    }

    int dequeue() {
        if (empty()) {
            throw std::out_of_range("Cannot dequeue from an empty queue");
        }
        // Read from the current head, then move the head pointer forward modulo capacity.
        int value = data[headIndex];
        headIndex = (headIndex + 1) % capacity;
        --count;
        return value;
    }

    int front() const {
        if (empty()) {
            throw std::out_of_range("Cannot peek at an empty queue");
        }
        // The logical front of the queue always lives at headIndex.
        return data[headIndex];
    }

    bool empty() const {
        return count == 0;
    }

    std::size_t size() const {
        return count;
    }

private:
    void grow() {
        std::size_t newCapacity = capacity * 2;
        int* newData = new int[newCapacity];
        for (std::size_t i = 0; i < count; ++i) {
            // Copy out elements in logical order so the queue remains consistent after resizing.
            newData[i] = data[(headIndex + i) % capacity];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
        // After copying to a fresh array, reset the ring indices to their canonical positions.
        headIndex = 0;
        tailIndex = count;
    }

    int* data = nullptr;
    // capacity reflects the allocated length of the circular buffer; headIndex/tailIndex delimit
    // the logical segment currently holding count elements.
    std::size_t capacity = 0;
    std::size_t headIndex = 0;
    std::size_t tailIndex = 0;
    std::size_t count = 0;
};

int main() {
    Queue queue;

    for (int value : {5, 9, 13, 17}) {
        // Load several elements to demonstrate the wraparound-friendly enqueue path.
        queue.enqueue(value);
    }

    std::cout << "Queue size after enqueues: " << queue.size() << "\n";
    std::cout << "Front element is: " << queue.front() << "\n";

    while (!queue.empty()) {
        // Drain the queue to show FIFO behavior where items emerge in the order inserted.
        std::cout << "Dequeued " << queue.dequeue() << ", size now " << queue.size() << "\n";
    }

    try {
        queue.dequeue();
    } catch (const std::out_of_range& ex) {
        std::cout << "Exception caught: " << ex.what() << "\n";
    }

    return 0;
}
