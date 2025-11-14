#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <tuple>

// Function to perform a query.
long long query(int u, int v) {
    if (u == v) return 0;
    std::cout << "? " << u << " " << v << std::endl;
    long long dist;
    std::cin >> dist;
    return dist;
}

void solve() {
    int n;
    std::cin >> n;

    if (n == 1) {
        std::cout << "!" << std::endl;
        return;
    }

    // Step 1: Pick an arbitrary node (e.g., 1) and find the furthest node from it.
    // This node, 'u', must be a leaf and an endpoint of some diameter.
    int u = 1;
    long long max_dist_from_1 = -1;
    // For n>1, there's always a node other than 1. If 1 is part of a 2-node tree, this loop finds u=2.
    // If n>2 and 1 is a leaf, this finds the node furthest from 1, which becomes an endpoint 'u'.
    for (int i = 2; i <= n; ++i) {
        long long d = query(1, i);
        if (d > max_dist_from_1) {
            max_dist_from_1 = d;
            u = i;
        }
    }

    // Step 2: Find the furthest node from 'u'. This node, 'v', is the other endpoint of a diameter.
    // While doing this, we get all distances from 'u'.
    std::vector<long long> dist_u(n + 1, 0);
    int v = -1;
    long long max_dist_from_u = -1;
    for (int i = 1; i <= n; ++i) {
        if (i == u) continue;
        dist_u[i] = query(u, i);
        if (dist_u[i] > max_dist_from_u) {
            max_dist_from_u = dist_u[i];
            v = i;
        }
    }
    
    long long diameter_len = dist_u[v];

    // Step 3: To identify all nodes on the diameter path and to find attachment points for other nodes,
    // we need distances from 'v' as well.
    std::vector<long long> dist_v(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        if (i == v || i == u) continue;
        dist_v[i] = query(v, i);
    }
    dist_v[u] = diameter_len;
    
    // Step 4: Identify vertices on the diameter path. A vertex i is on the u-v path iff
    // dist(u,i) + dist(v,i) == dist(u,v).
    std::vector<std::pair<long long, int>> path_nodes;
    std::vector<bool> on_path(n + 1, false);
    for (int i = 1; i <= n; ++i) {
        if (dist_u[i] + dist_v[i] == diameter_len) {
            path_nodes.push_back({dist_u[i], i});
            on_path[i] = true;
        }
    }
    
    // Step 5: Sort path vertices by distance from 'u' to determine the path structure.
    // Then, add the edges of the diameter path to our answer set.
    std::sort(path_nodes.begin(), path_nodes.end());
    std::vector<std::tuple<int, int, int>> edges;
    for (size_t i = 0; i < path_nodes.size() - 1; ++i) {
        int U = path_nodes[i].second;
        int V = path_nodes[i+1].second;
        long long W = path_nodes[i+1].first - path_nodes[i].first;
        edges.emplace_back(U, V, (int)W);
    }
    
    // Step 6: Attach the remaining (off-path) vertices.
    if (path_nodes.size() < (size_t)n) {
        std::map<long long, int> diff_to_node;
        for (const auto& p_pair : path_nodes) {
            long long d_u_p = p_pair.first;
            int p_node = p_pair.second;
            // The unique identifier is dist(u,p) - dist(v,p), which simplifies to:
            // dist(u,p) - (diameter_len - dist(u,p)) = 2*dist(u,p) - diameter_len
            diff_to_node[2 * d_u_p - diameter_len] = p_node;
        }
        
        for (int i = 1; i <= n; ++i) {
            if (!on_path[i]) {
                long long diff_k = dist_u[i] - dist_v[i];
                int p = diff_to_node[diff_k];
                long long W = dist_u[i] - dist_u[p];
                edges.emplace_back(i, p, (int)W);
            }
        }
    }
    
    // Step 7: Output the final answer.
    std::cout << "!";
    for (const auto& edge : edges) {
        std::cout << " " << std::get<0>(edge) << " " << std::get<1>(edge) << " " << std::get<2>(edge);
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int T;
    std::cin >> T;
    while (T--) {
        solve();
    }
    return 0;
}