#include <algorithm>
#include <iostream>
#include <vector>

// This program generates all permutations of a sequence using backtracking.
// Backtracking explores the search tree by choosing an element for the current position,
// recursing on the remaining positions, and undoing the choice before trying the next option.
// Swapping in place keeps the data structure compact: the recursion depth equals the prefix length.

void generatePermutations(std::vector<int>& items, size_t index) {
    if (index == items.size()) {
        for (int value : items) {
            std::cout << value << ' ';
        }
        std::cout << "\n";
        return;
    }
    for (size_t i = index; i < items.size(); ++i) {
        std::swap(items[index], items[i]);
        generatePermutations(items, index + 1);
        std::swap(items[index], items[i]); // Backtrack to restore original order.
    }
}

int main() {
    std::vector<int> values = {1, 2, 3};
    generatePermutations(values, 0);
    return 0;
}
