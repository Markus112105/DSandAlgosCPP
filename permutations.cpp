#include <algorithm>
#include <iostream>
#include <vector>

// This program generates all permutations of a sequence using backtracking.
// Backtracking explores the search tree by choosing an element for the current position,
// recursing on the remaining positions, and undoing the choice before trying the next option.
// Swapping in place keeps the data structure compact: the recursion depth equals the prefix length.

void generatePermutations(std::vector<int>& items, size_t index) {
    // Once the prefix covers the entire array, we have a full permutation to report.
    if (index == items.size()) {
        for (int value : items) {
            std::cout << value << ' ';
        }
        std::cout << "\n";
        return;
    }
    for (size_t i = index; i < items.size(); ++i) {
        // Swap the candidate element into the current position so the prefix reflects our choice.
        std::swap(items[index], items[i]);
        // Recurse to arrange the remaining suffix; the vector itself acts as mutable working storage.
        generatePermutations(items, index + 1);
        std::swap(items[index], items[i]); // Backtrack to restore original order.
    }
}

int main() {
    std::vector<int> values = {1, 2, 3};
    // Kick off the search with an empty prefix; recursion will explore the factorial search space.
    generatePermutations(values, 0);
    return 0;
}
