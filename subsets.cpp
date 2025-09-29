#include <iostream>
#include <vector>

// This program enumerates all subsets (the power set) of a small collection of integers.
// The algorithm uses recursion to decide for each position whether to include the element.
// This approach is effectively walking a binary decision tree of height n, yielding 2^n subsets.

void generateSubsets(const std::vector<int>& items, size_t index, std::vector<int>& current) {
    if (index == items.size()) {
        std::cout << '{';
        for (size_t i = 0; i < current.size(); ++i) {
            std::cout << current[i];
            if (i + 1 < current.size()) {
                std::cout << ',';
            }
        }
        std::cout << "}\n";
        return;
    }

    // Include the current item.
    current.push_back(items[index]);
    generateSubsets(items, index + 1, current);
    current.pop_back();

    // Exclude the current item.
    generateSubsets(items, index + 1, current);
}

int main() {
    std::vector<int> values = {1, 2, 3};
    std::vector<int> current;

    generateSubsets(values, 0, current);

    return 0;
}
