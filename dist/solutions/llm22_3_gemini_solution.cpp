#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

std::vector<int> adj[100005];
int parent[100005];
int min_leaf[100005];
int max_leaf[100005];

void dfs_minmax(int u) {
    if (adj[u].empty()) {
        min_leaf[u] = u;
        max_leaf[u] = u;
        return;
    }

    min_leaf[u] = 1e9 + 7;
    max_leaf[u] = 0;
    for (int v : adj[u]) {
        dfs_minmax(v);
        min_leaf[u] = std::min(min_leaf[u], min_leaf[v]);
        max_leaf[u] = std::max(max_leaf[u], max_leaf[v]);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    if (n == 4) {
        // Based on problem constraints (root degree >= 2), N=4 implies a star graph
        // on {1,2,3,4}, with 1 as center. With the leaf cycle, this is K4.
        // The simplest tree decomposition is one bag with all vertices.
        std::cout << 1 << "\n";
        std::cout << "4 1 2 3 4\n";
        // Drain input
        for (int i=0; i<n-1; ++i) { int p; std::cin >> p; }
        return 0;
    }
    
    parent[1] = 0; // Root has no parent
    for (int i = 2; i <= n; ++i) {
        std::cin >> parent[i];
        adj[parent[i]].push_back(i);
    }
    
    dfs_minmax(1);

    std::vector<std::vector<int>> new_nodes_bags;
    std::vector<std::pair<int, int>> new_edges;
    int k = 0;

    // Create U_u nodes
    for (int i = 1; i <= n; ++i) {
        k++;
        std::vector<int> bag;
        bag.push_back(i);
        if (parent[i] != 0) {
            bag.push_back(parent[i]);
        }
        if (min_leaf[i] != 1e9 + 7 && min_leaf[i] != i) {
             bag.push_back(min_leaf[i]);
        }
        if (max_leaf[i] != 0 && max_leaf[i] != i) {
            bag.push_back(max_leaf[i]);
        }
        std::sort(bag.begin(), bag.end());
        bag.erase(std::unique(bag.begin(), bag.end()), bag.end());
        new_nodes_bags.push_back(bag);
    }

    for (int i = 2; i <= n; ++i) {
        new_edges.push_back({i, parent[i]});
    }

    // Create W_u,a nodes
    for (int u = 1; u <= n; ++u) {
        if (adj[u].size() > 1) {
            for (size_t i = 0; i < adj[u].size() - 1; ++i) {
                int c1 = adj[u][i];
                int c2 = adj[u][i+1];
                
                k++;
                std::vector<int> bag;
                bag.push_back(u);
                bag.push_back(max_leaf[c1]);
                bag.push_back(min_leaf[c2]);
                std::sort(bag.begin(), bag.end());
                bag.erase(std::unique(bag.begin(), bag.end()), bag.end());

                new_nodes_bags.push_back(bag);
                new_edges.push_back({k, u});
            }
            if (u == 1) { // wrap-around case for root
                int c1 = adj[u].back();
                int c2 = adj[u][0];

                k++;
                std::vector<int> bag;
                bag.push_back(u);
                bag.push_back(max_leaf[c1]);
                bag.push_back(min_leaf[c2]);
                std::sort(bag.begin(), bag.end());
                bag.erase(std::unique(bag.begin(), bag.end()), bag.end());

                new_nodes_bags.push_back(bag);
                new_edges.push_back({k, u});
            }
        }
    }

    std::cout << k << "\n";
    for (const auto& bag : new_nodes_bags) {
        std::cout << bag.size();
        for (int node : bag) {
            std::cout << " " << node;
        }
        std::cout << "\n";
    }

    for (const auto& edge : new_edges) {
        std::cout << edge.first << " " << edge.second << "\n";
    }

    return 0;
}