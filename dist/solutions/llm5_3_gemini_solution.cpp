#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

void fast_io() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
}

int main() {
    fast_io();

    int n, m;
    std::cin >> n >> m;

    // The scoring parameters are not needed for the logic but must be read from input.
    std::vector<int> a(10);
    for (int i = 0; i < 10; ++i) {
        std::cin >> a[i];
    }

    std::vector<std::vector<int>> adj(n + 1);
    std::vector<int> in_degree(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        in_degree[v]++;
    }

    // Heuristically find the best starting node: one with the minimum in-degree.
    // Tie-break with the smallest vertex ID.
    int start_node = -1;
    int min_in = n + 1; // Initialize with a value larger than any possible in-degree
    for (int i = 1; i <= n; ++i) {
        if (in_degree[i] < min_in) {
            min_in = in_degree[i];
            start_node = i;
        }
    }

    std::vector<int> path;
    std::vector<bool> visited(n + 1, false);
    int current_node = start_node;

    // Greedily build the path for at most n steps.
    for (int i = 0; i < n && current_node != -1; ++i) {
        path.push_back(current_node);
        visited[current_node] = true;

        int best_next = -1;
        
        // Find the best next vertex: unvisited neighbor with the smallest in-degree.
        // Tie-break with the smallest vertex ID.
        for (int neighbor : adj[current_node]) {
            if (!visited[neighbor]) {
                if (best_next == -1 ||
                    in_degree[neighbor] < in_degree[best_next] ||
                    (in_degree[neighbor] == in_degree[best_next] && neighbor < best_next)) {
                    best_next = neighbor;
                }
            }
        }
        current_node = best_next;
    }

    std::cout << path.size() << "\n";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i] << (i == path.size() - 1 ? "" : " ");
    }
    std::cout << "\n";

    return 0;
}