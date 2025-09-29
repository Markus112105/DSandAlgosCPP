#include <iostream>
#include <vector>

// This program prints all k-combinations of a given set using recursive backtracking.
// At each recursion level we decide whether to include the current element, advancing
// through the array while tracking how many elements are left to pick. The active selection
// is stored in a separate vector so we can output combinations in lexicographic order.

void generateCombinations(const std::vector<int>& items, size_t index, int k,
                          std::vector<int>& current) {
    // When the working selection reaches size k, the recursion has picked enough elements
    // to form one combination, so emit it immediately.
    if (current.size() == static_cast<size_t>(k)) {
        for (int value : current) {
            std::cout << value << ' ';
        }
        std::cout << "\n";
        return;
    }
    // If we run out of candidates before hitting k picks, the branch fails silently.
    if (index == items.size()) {
        return;
    }

    // Include the item at this index, growing the partial combination.
    current.push_back(items[index]);
    generateCombinations(items, index + 1, k, current);
    current.pop_back();

    // Exclude the same item to explore the complementary branch of the decision tree.
    generateCombinations(items, index + 1, k, current);
}

int main() {
    std::vector<int> values = {1, 2, 3, 4};
    int k = 2;

    std::vector<int> current;
    // Start the recursion with an empty slate, trusting the helper to expand the search tree.
    generateCombinations(values, 0, k, current);

    return 0;
}
