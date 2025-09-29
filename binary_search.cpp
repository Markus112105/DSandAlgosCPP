#include <iostream>
#include <vector>

// This program illustrates binary search, a logarithmic-time algorithm for finding a
// target value inside a sorted array. We implement both iterative and recursive variants.
// Binary search repeatedly halves the search interval, using the array's ordering to decide
// which half might contain the target while discarding the other half.

int binarySearchIterative(const std::vector<int>& data, int target) {
    // The window [left, right] captures the portion of the sorted vector that may still
    // contain the target. Shrinking this interval logarithmically gives O(log n) runtime.
    int left = 0;
    int right = static_cast<int>(data.size()) - 1;
    while (left <= right) {
        // Choosing the midpoint with the overflow-safe formula keeps the search balanced.
        int mid = left + (right - left) / 2;
        if (data[mid] == target) {
            return mid;
        }
        if (data[mid] < target) {
            // Discard the left half, since all values there are smaller than target.
            left = mid + 1;
        } else {
            // Discard the right half, since all values there are larger than target.
            right = mid - 1;
        }
    }
    return -1;
}

int binarySearchRecursive(const std::vector<int>& data, int target, int left, int right) {
    // When the interval becomes empty, the value was not found anywhere in the array.
    if (left > right) {
        return -1;
    }
    // Midpoint selection mirrors the iterative routine so both variants share behavior.
    int mid = left + (right - left) / 2;
    if (data[mid] == target) {
        return mid;
    }
    if (data[mid] < target) {
        // Recurse on the right subtree of the conceptual search tree.
        return binarySearchRecursive(data, target, mid + 1, right);
    }
    // Otherwise recurse on the left subtree.
    return binarySearchRecursive(data, target, left, mid - 1);
}

int main() {
    std::vector<int> sorted = {1, 3, 5, 7, 9, 11, 13, 15};

    // Demonstrate that both implementations return matching positions for present values
    // and -1 when the searched key is missing.
    std::cout << "Iterative search for 7: index " << binarySearchIterative(sorted, 7) << "\n";
    std::cout << "Recursive search for 11: index "
              << binarySearchRecursive(sorted, 11, 0, static_cast<int>(sorted.size()) - 1) << "\n";
    std::cout << "Search for missing value 4: index " << binarySearchIterative(sorted, 4) << "\n";

    return 0;
}
