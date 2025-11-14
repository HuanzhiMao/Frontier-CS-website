#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

int n, ty;

// Function to perform a query to the black box.
int ask(const std::vector<int>& vec) {
    std::cout << "? " << vec.size();
    for (int x : vec) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
    int result;
    std::cin >> result;
    return result;
}

// Cache for ancestor checks to avoid re-computing for the same pair.
std::map<std::pair<int, int>, bool> ancestor_cache;

// Determines if node u is an ancestor of node v.
// Uses a fixed permutation of other nodes to provide a consistent background for comparison.
bool is_ancestor(int u, int v, const std::vector<int>& all_nodes_perm) {
    if (u == v) return false;
    if (ancestor_cache.count({u, v})) {
        return ancestor_cache[{u, v}];
    }

    // A simple 2-node query can check for any ancestor-descendant relationship.
    if (ask({u, v}) == 2) {
        return ancestor_cache[{u, v}] = false;
    }

    // To determine direction, we prepend (u,v) and (v,u) to a permutation of all other nodes.
    // The ancestor will block more nodes, resulting in a smaller greedy set size.
    std::vector<int> query_uv;
    query_uv.push_back(u);
    query_uv.push_back(v);
    for(int node : all_nodes_perm) {
        if(node != u && node != v) query_uv.push_back(node);
    }

    std::vector<int> query_vu;
    query_vu.push_back(v);
    query_vu.push_back(u);
    for(int node : all_nodes_perm) {
        if(node != u && node != v) query_vu.push_back(node);
    }

    int res_uv = ask(query_uv);
    int res_vu = ask(query_vu);

    return ancestor_cache[{u, v}] = (res_uv < res_vu);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n >> ty;

    std::vector<int> all_nodes_perm(n);
    std::iota(all_nodes_perm.begin(), all_nodes_perm.end(), 1);

    // Step 1: Establish a topological sort of nodes.
    // Query each node first, followed by all others. Ancestors will have smaller results.
    std::vector<std::pair<int, int>> s_values(n);
    for (int i = 1; i <= n; ++i) {
        std::vector<int> q_vec;
        q_vec.push_back(i);
        for (int j = 1; j <= n; ++j) {
            if (i == j) continue;
            q_vec.push_back(j);
        }
        s_values[i - 1] = {ask(q_vec), i};
    }

    std::sort(s_values.begin(), s_values.end());

    std::vector<int> sorted_nodes(n);
    for (int i = 0; i < n; ++i) {
        sorted_nodes[i] = s_values[i].second;
    }

    // Step 2: Reconstruct the tree using the topological order.
    std::vector<int> par(n + 1, 0);
    int root = sorted_nodes[0];
    par[root] = 0;

    for (int i = 1; i < n; ++i) {
        int u = sorted_nodes[i];
        int prev_u = sorted_nodes[i - 1];

        // The parent of u is the LCA of u and prev_u.
        // We can find this by searching on the path from prev_u to the root.
        std::vector<int> path;
        int curr = prev_u;
        while (curr != 0) {
            path.push_back(curr);
            curr = par[curr];
        }

        // Path is from prev_u to root (deep to shallow).
        // Binary search for the first node on this path that is an ancestor of u.
        // This will be the deepest such node, i.e., the LCA.
        int low = 0, high = path.size() - 1;
        int p_cand = path.back(); // Default to root

        while (low <= high) {
            int mid_idx = low + (high - low) / 2;
            if (is_ancestor(path[mid_idx], u, all_nodes_perm)) {
                p_cand = path[mid_idx];
                high = mid_idx - 1; // Try to find an even deeper one
            } else {
                low = mid_idx + 1;
            }
        }
        par[u] = p_cand;
    }

    // Step 3: Output the answer.
    std::cout << "!";
    for (int i = 1; i <= n; ++i) {
        std::cout << " " << par[i];
    }
    std::cout << std::endl;

    return 0;
}