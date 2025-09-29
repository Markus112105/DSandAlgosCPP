#include <iostream>
#include <vector>

// This program showcases merge sort on an integer vector. Merge sort uses a divide-and-conquer
// strategy: split the array into halves, recursively sort each half, then merge the two sorted
// halves into a single sorted run. The merging stage maintains order by repeatedly taking the
// smallest remaining element from either half, yielding O(n log n) time with stable behavior.

void merge(std::vector<int>& values, std::vector<int>& buffer, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (values[i] <= values[j]) {
            buffer[k++] = values[i++];
        } else {
            buffer[k++] = values[j++];
        }
    }

    while (i <= mid) {
        buffer[k++] = values[i++];
    }

    while (j <= right) {
        buffer[k++] = values[j++];
    }

    for (int idx = left; idx <= right; ++idx) {
        values[idx] = buffer[idx];
    }
}

void mergeSortRecursive(std::vector<int>& values, std::vector<int>& buffer, int left, int right) {
    if (left >= right) {
        return;
    }
    int mid = left + (right - left) / 2;
    mergeSortRecursive(values, buffer, left, mid);
    mergeSortRecursive(values, buffer, mid + 1, right);
    merge(values, buffer, left, mid, right);
}

void mergeSort(std::vector<int>& values) {
    if (values.empty()) {
        return;
    }
    std::vector<int> buffer(values.size());
    mergeSortRecursive(values, buffer, 0, static_cast<int>(values.size()) - 1);
}

int main() {
    std::vector<int> data = {38, 27, 43, 3, 9, 82, 10};

    std::cout << "Before sort: ";
    for (int number : data) {
        std::cout << number << ' ';
    }
    std::cout << "\n";

    mergeSort(data);

    std::cout << "After merge sort: ";
    for (int number : data) {
        std::cout << number << ' ';
    }
    std::cout << "\n";

    return 0;
}
