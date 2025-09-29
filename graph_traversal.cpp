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
        // Each adjacency entry stores neighbors in a vector so we can iterate quickly.
        adjacency[from].push_back(to);
        adjacency[to].push_back(from); // Undirected graph for illustration.
    }

    void depthFirstSearch(int start) const {
        // The visited map tracks which vertices have already been processed to avoid cycles.
        std::unordered_map<int, bool> visited;
        // Using an explicit stack replicates the call stack of a recursive DFS but keeps control iterative.
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
            // Push neighbors in reverse to preserve intuitive left-to-right visiting order.
            for (auto rit = neighbors.rbegin(); rit != neighbors.rend(); ++rit) {
                if (!visited[*rit]) {
                    stack.push(*rit);
                }
            }
        }
        std::cout << "\n";
    }

    void breadthFirstSearch(int start) const {
        // BFS leverages a queue so vertices are processed in the order they are discovered.
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
            // Newly discovered vertices are enqueued, ensuring their breadth-first distance ordering.
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
    // The adjacency list maps each vertex to a collection of immediate neighbors.
    std::unordered_map<int, std::vector<int>> adjacency;
};

int main() {
    Graph graph;
    // Build a small binary-like adjacency structure to illustrate how DFS and BFS diverge.
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
