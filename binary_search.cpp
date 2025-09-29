#include <iostream>
#include <vector>

// This program illustrates binary search, a logarithmic-time algorithm for finding a
// target value inside a sorted array. We implement both iterative and recursive variants.
// Binary search repeatedly halves the search interval, using the array's ordering to decide
// which half might contain the target while discarding the other half.

int binarySearchIterative(const std::vector<int>& data, int target) {
    int left = 0;
    int right = static_cast<int>(data.size()) - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (data[mid] == target) {
            return mid;
        }
        if (data[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

int binarySearchRecursive(const std::vector<int>& data, int target, int left, int right) {
    if (left > right) {
        return -1;
    }
    int mid = left + (right - left) / 2;
    if (data[mid] == target) {
        return mid;
    }
    if (data[mid] < target) {
        return binarySearchRecursive(data, target, mid + 1, right);
    }
    return binarySearchRecursive(data, target, left, mid - 1);
}

int main() {
    std::vector<int> sorted = {1, 3, 5, 7, 9, 11, 13, 15};

    std::cout << "Iterative search for 7: index " << binarySearchIterative(sorted, 7) << "\n";
    std::cout << "Recursive search for 11: index "
              << binarySearchRecursive(sorted, 11, 0, static_cast<int>(sorted.size()) - 1) << "\n";
    std::cout << "Search for missing value 4: index " << binarySearchIterative(sorted, 4) << "\n";

    return 0;
}
