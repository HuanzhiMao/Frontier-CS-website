#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

void fast_io() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
}

int n, m;
std::vector<std::vector<int>> adj;
std::vector<int> out_degree;
std::vector<int> best_path;

void search_from(int start_node) {
    std::vector<int> current_path;
    std::vector<bool> visited(n + 1, false);
    
    int current_node = start_node;
    
    while (current_node != -1) {
        visited[current_node] = true;
        current_path.push_back(current_node);
        
        int next_node = -1;
        int max_od = -1;
        
        // Greedily pick the unvisited neighbor with the highest out-degree.
        // Tie-breaking: smaller vertex index (achieved by sorting adj lists).
        for (int neighbor : adj[current_node]) {
            if (!visited[neighbor]) {
                if (out_degree[neighbor] > max_od) {
                    max_od = out_degree[neighbor];
                    next_node = neighbor;
                }
            }
        }
        current_node = next_node;
    }
    
    if (current_path.size() > best_path.size()) {
        best_path = current_path;
    }
}

int main() {
    fast_io();

    std::cin >> n >> m;

    for (int i = 0; i < 10; ++i) {
        int a;
        std::cin >> a;
    }

    adj.resize(n + 1);
    std::vector<int> in_degree(n + 1, 0);
    out_degree.assign(n + 1, 0);

    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        out_degree[u]++;
        in_degree[v]++;
    }
    
    for (int i = 1; i <= n; ++i) {
        std::sort(adj[i].begin(), adj[i].end());
    }

    std::vector<int> start_nodes;
    for (int i = 1; i <= n; ++i) {
        if (in_degree[i] == 0) {
            start_nodes.push_back(i);
        }
    }
    
    if (start_nodes.empty() && n > 0) {
        int min_in_degree = n + 1;
        for (int i = 1; i <= n; ++i) {
            if (in_degree[i] < min_in_degree) {
                min_in_degree = in_degree[i];
            }
        }
        for (int i = 1; i <= n; ++i) {
            if (in_degree[i] == min_in_degree) {
                start_nodes.push_back(i);
            }
        }
    }
    
    for (int start_node : start_nodes) {
        search_from(start_node);
        if (best_path.size() == n) {
            break;
        }
    }
    
    // As a fallback if no path was found (e.g. n=0 or no start_nodes logic worked)
    if (best_path.empty() && n > 0) {
        search_from(1);
    }


    std::cout << best_path.size() << "\n";
    for (size_t i = 0; i < best_path.size(); ++i) {
        std::cout << best_path[i] << (i == best_path.size() - 1 ? "" : " ");
    }
    std::cout << "\n";

    return 0;
}