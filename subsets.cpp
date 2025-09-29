#include <iostream>
#include <vector>

// This program enumerates all subsets (the power set) of a small collection of integers.
// The algorithm uses recursion to decide for each position whether to include the element.
// This approach is effectively walking a binary decision tree of height n, yielding 2^n subsets.

void generateSubsets(const std::vector<int>& items, size_t index, std::vector<int>& current) {
    // When index reaches items.size(), we have committed to include/exclude every element once.
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

    // Branch: include the element at this index in the working subset.
    current.push_back(items[index]);
    generateSubsets(items, index + 1, current);
    current.pop_back();

    // Branch: skip the same element and explore the complementary decision path.
    generateSubsets(items, index + 1, current);
}

int main() {
    std::vector<int> values = {1, 2, 3};
    std::vector<int> current;

    // Kick off the recursion with no selections; current is mutated in place as branches explore sets.
    generateSubsets(values, 0, current);

    return 0;
}
