#include <iostream>
#include <vector>

// This program prints all k-combinations of a given set using recursive backtracking.
// At each recursion level we decide whether to include the current element, advancing
// through the array while tracking how many elements are left to pick. The active selection
// is stored in a separate vector so we can output combinations in lexicographic order.

void generateCombinations(const std::vector<int>& items, size_t index, int k,
                          std::vector<int>& current) {
    if (current.size() == static_cast<size_t>(k)) {
        for (int value : current) {
            std::cout << value << ' ';
        }
        std::cout << "\n";
        return;
    }
    if (index == items.size()) {
        return;
    }

    // Choose the item at "index".
    current.push_back(items[index]);
    generateCombinations(items, index + 1, k, current);
    current.pop_back();

    // Skip the item at "index" and move on.
    generateCombinations(items, index + 1, k, current);
}

int main() {
    std::vector<int> values = {1, 2, 3, 4};
    int k = 2;

    std::vector<int> current;
    generateCombinations(values, 0, k, current);

    return 0;
}
