#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

// This program demonstrates Depth-First Search (DFS) and Breadth-First Search (BFS) on an
// unweighted graph. The graph is stored as an adjacency list using an unordered_map where each
// vertex maps to its neighbors. This structure keeps lookups near O(1) average time and is
// space-efficient for sparse graphs. DFS uses an explicit stack to dive deep, while BFS uses a
// queue to explore level by level.

class Graph {
public:
    void addEdge(int from, int to) {
        adjacency[from].push_back(to);
        adjacency[to].push_back(from); // Undirected graph for illustration.
    }

    void depthFirstSearch(int start) const {
        std::unordered_map<int, bool> visited;
        std::stack<int> stack;
        stack.push(start);

        std::cout << "DFS order: ";

        while (!stack.empty()) {
            int vertex = stack.top();
            stack.pop();

            if (visited[vertex]) {
                continue;
            }
            visited[vertex] = true;
            std::cout << vertex << ' ';

            auto it = adjacency.find(vertex);
            if (it == adjacency.end()) {
                continue;
            }
            const auto& neighbors = it->second;
            for (auto rit = neighbors.rbegin(); rit != neighbors.rend(); ++rit) {
                if (!visited[*rit]) {
                    stack.push(*rit);
                }
            }
        }
        std::cout << "\n";
    }

    void breadthFirstSearch(int start) const {
        std::unordered_map<int, bool> visited;
        std::queue<int> queue;
        queue.push(start);
        visited[start] = true;

        std::cout << "BFS order: ";

        while (!queue.empty()) {
            int vertex = queue.front();
            queue.pop();
            std::cout << vertex << ' ';

            auto it = adjacency.find(vertex);
            if (it == adjacency.end()) {
                continue;
            }
            const auto& neighbors = it->second;
            for (int neighbor : neighbors) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    queue.push(neighbor);
                }
            }
        }
        std::cout << "\n";
    }

private:
    std::unordered_map<int, std::vector<int>> adjacency;
};

int main() {
    Graph graph;
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 4);
    graph.addEdge(2, 5);
    graph.addEdge(3, 6);
    graph.addEdge(3, 7);

    graph.depthFirstSearch(1);
    graph.breadthFirstSearch(1);

    return 0;
}
