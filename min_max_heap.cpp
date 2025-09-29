#include <iostream>
#include <stdexcept>
#include <vector>

// This program demonstrates a Min-Max Heap, a double-ended priority queue that allows
// both minimum and maximum elements to be accessed or removed in logarithmic time.
// The heap stores elements in a vector representing a complete binary tree. Even-depth
// levels obey min-heap ordering and odd-depth levels obey max-heap ordering, allowing us
// to reach both extremes quickly without maintaining two separate heaps.
class MinMaxHeap {
public:
    void insert(int value) {
        // Append at the end to satisfy the array-based complete tree layout, then repair ordering.
        data.push_back(value);
        bubbleUp(data.size() - 1);
    }

    int getMin() const {
        if (data.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return data.front();
    }

    int getMax() const {
        if (data.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        if (data.size() == 1) {
            return data.front();
        }
        // The maximum element must be one of the root's children since level one stores the largest keys.
        if (data.size() == 2) {
            return data[1];
        }
        return std::max(data[1], data[2]);
    }

    int extractMin() {
        if (data.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        int minValue = data.front();
        data[0] = data.back();
        data.pop_back();
        if (!data.empty()) {
            // After moving the last element to the root, push it downward to restore heap invariants.
            trickleDown(0);
        }
        return minValue;
    }

    int extractMax() {
        if (data.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        if (data.size() == 1) {
            int maxValue = data.front();
            data.pop_back();
            return maxValue;
        }
        size_t maxIndex = 1;
        if (data.size() > 2 && data[2] > data[1]) {
            maxIndex = 2;
        }
        int maxValue = data[maxIndex];
        data[maxIndex] = data.back();
        data.pop_back();
        if (maxIndex < data.size()) {
            // Trickle from the original child position because we replaced that slot with the last element.
            trickleDown(maxIndex);
        }
        return maxValue;
    }

    bool empty() const {
        return data.empty();
    }

private:
    // Array-backed complete binary tree where indices encode parent/child relationships.
    std::vector<int> data;

    static size_t parent(size_t index) {
        return (index - 1) / 2;
    }

    static size_t grandparent(size_t index) {
        return parent(parent(index));
    }

    bool isMinLevel(size_t index) const {
        // Compute the depth of the node; even depths obey min-heap ordering, odd depths obey max ordering.
        size_t depth = 0;
        while (index > 0) {
            index = parent(index);
            ++depth;
        }
        return depth % 2 == 0;
    }

    // Bubble-up fixes ordering after insertions by moving a node toward the root until it satisfies
    // the min/max ordering for its level.
    void bubbleUp(size_t index) {
        if (index == 0) {
            return;
        }
        size_t p = parent(index);
        if (isMinLevel(index)) {
            if (data[index] > data[p]) {
                // Value is too large for a min level, so swap with parent and enforce max-level rules above.
                std::swap(data[index], data[p]);
                bubbleUpMax(p);
            } else {
                // Value respects the min-level constraint; move up through grandparent chain if needed.
                bubbleUpMin(index);
            }
        } else {
            if (data[index] < data[p]) {
                // Value is too small for a max level; swap and propagate along the min-level ancestors.
                std::swap(data[index], data[p]);
                bubbleUpMin(p);
            } else {
                // Otherwise climb the max-level chain to position larger values nearer the top of that level.
                bubbleUpMax(index);
            }
        }
    }

    void bubbleUpMin(size_t index) {
        while (index >= 3) {
            size_t gp = grandparent(index);
            if (data[index] < data[gp]) {
                // Grandparent lives on the same min layer, so swap to move the small value upward.
                std::swap(data[index], data[gp]);
                index = gp;
            } else {
                break;
            }
        }
    }

    void bubbleUpMax(size_t index) {
        while (index >= 3) {
            size_t gp = grandparent(index);
            if (data[index] > data[gp]) {
                // Promote larger values up the max layer by leapfrogging over the parent.
                std::swap(data[index], data[gp]);
                index = gp;
            } else {
                break;
            }
        }
    }

    void trickleDown(size_t index) {
        if (isMinLevel(index)) {
            trickleDownMin(index);
        } else {
            trickleDownMax(index);
        }
    }

    // Trickle-down pushes a node downward until all descendants satisfy the ordering.
    void trickleDownMin(size_t index) {
        while (true) {
            size_t m = minDescendant(index);
            if (m == index) {
                return;
            }
            if (isGrandchild(index, m)) {
                if (data[m] < data[index]) {
                    std::swap(data[m], data[index]);
                    size_t parentIdx = parent(m);
                    if (data[m] > data[parentIdx]) {
                        // After a grandchild swap, ensure the parent still dominates as a max-level node.
                        std::swap(data[m], data[parentIdx]);
                    }
                    index = m;
                } else {
                    return;
                }
            } else {
                if (data[m] < data[index]) {
                    std::swap(data[m], data[index]);
                }
                return;
            }
        }
    }

    void trickleDownMax(size_t index) {
        while (true) {
            size_t m = maxDescendant(index);
            if (m == index) {
                return;
            }
            if (isGrandchild(index, m)) {
                if (data[m] > data[index]) {
                    std::swap(data[m], data[index]);
                    size_t parentIdx = parent(m);
                    if (data[m] < data[parentIdx]) {
                        // After bubbling up a larger grandchild, fix any inversion with its direct parent.
                        std::swap(data[m], data[parentIdx]);
                    }
                    index = m;
                } else {
                    return;
                }
            } else {
                if (data[m] > data[index]) {
                    std::swap(data[m], data[index]);
                }
                return;
            }
        }
    }

    bool isGrandchild(size_t index, size_t descendant) const {
        if (descendant == index) {
            return false;
        }
        size_t childStart = 2 * index + 1;
        size_t childEnd = 2 * index + 2;
        if (descendant >= data.size()) {
            return false;
        }
        if (descendant == childStart || descendant == childEnd) {
            return false;
        }
        size_t p = parent(descendant);
        // A node is a grandchild if its parent is one of the direct children of the starting index.
        return p == childStart || p == childEnd;
    }

    size_t minDescendant(size_t index) const {
        size_t firstChild = 2 * index + 1;
        if (firstChild >= data.size()) {
            return index;
        }
        size_t minIdx = firstChild;
        size_t lastDescendant = std::min(data.size() - 1, 2 * firstChild + 2);
        for (size_t i = firstChild; i <= lastDescendant; ++i) {
            if (data[i] < data[minIdx]) {
                // Examine children and grandchildren to find the smallest candidate under this node.
                minIdx = i;
            }
        }
        return minIdx;
    }

    size_t maxDescendant(size_t index) const {
        size_t firstChild = 2 * index + 1;
        if (firstChild >= data.size()) {
            return index;
        }
        size_t maxIdx = firstChild;
        size_t lastDescendant = std::min(data.size() - 1, 2 * firstChild + 2);
        for (size_t i = firstChild; i <= lastDescendant; ++i) {
            if (data[i] > data[maxIdx]) {
                // Symmetric to minDescendant: pick the largest among children and grandchildren.
                maxIdx = i;
            }
        }
        return maxIdx;
    }
};

int main() {
    MinMaxHeap heap;
    heap.insert(10);
    heap.insert(5);
    heap.insert(30);
    heap.insert(3);
    heap.insert(17);
    heap.insert(22);

    // The demonstrations exercise both ends to underscore the double-ended priority queue behavior.
    std::cout << "Min: " << heap.getMin() << "\n";
    std::cout << "Max: " << heap.getMax() << "\n";

    std::cout << "Extracted Min: " << heap.extractMin() << "\n";
    std::cout << "Extracted Max: " << heap.extractMax() << "\n";

    std::cout << "Min after extractions: " << heap.getMin() << "\n";
    std::cout << "Max after extractions: " << heap.getMax() << "\n";

    return 0;
}
