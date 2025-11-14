#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

const int MAXN = 1005;
const int LOGN = 11;

std::vector<std::pair<int, int>> adj[MAXN];
int p[MAXN];
int n;

int parent[MAXN];
int depth[MAXN];
int tin[MAXN], tout[MAXN];
int up[MAXN][LOGN];
int timer;

void dfs_lca(int u, int p, int d) {
    tin[u] = ++timer;
    parent[u] = p;
    depth[u] = d;
    up[u][0] = p;
    for (int i = 1; i < LOGN; ++i) {
        up[u][i] = up[up[u][i - 1]][i - 1];
    }
    for (auto& edge : adj[u]) {
        int v = edge.first;
        if (v != p) {
            dfs_lca(v, u, d + 1);
        }
    }
    tout[u] = ++timer;
}

bool is_ancestor(int u, int v) {
    if (u == 0) return true;
    if (v == 0) return false;
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int get_ancestor(int u, int k) {
    for (int i = 0; i < LOGN; ++i) {
        if ((k >> i) & 1) {
            u = up[u][i];
        }
    }
    return u;
}

int next_hop(int u, int target) {
    if (u == target) return u;
    if (is_ancestor(u, target)) {
        return get_ancestor(target, depth[target] - depth[u] - 1);
    } else {
        return parent[u];
    }
}

bool is_sorted() {
    for (int i = 1; i <= n; ++i) {
        if (p[i] != i) {
            return false;
        }
    }
    return true;
}

void solve() {
    std::cin >> n;
    for (int i = 1; i <= n; ++i) {
        std::cin >> p[i];
    }
    for (int i = 1; i <= n; ++i) {
        adj[i].clear();
    }
    std::map<std::pair<int, int>, int> edge_to_idx;
    for (int i = 1; i < n; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
        if (u > v) std::swap(u, v);
        edge_to_idx[{u, v}] = i;
    }

    timer = 0;
    dfs_lca(1, 0, 0);

    std::vector<std::vector<int>> operations;
    while (!is_sorted()) {
        std::vector<int> desire(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            if (p[i] != i) {
                desire[i] = next_hop(i, p[i]);
            }
        }

        std::vector<int> current_matching_indices;
        std::vector<bool> used(n + 1, false);
        
        // Bidirectional pass
        for (int u = 1; u <= n; ++u) {
            if (!used[u] && desire[u] != 0) {
                int v = desire[u];
                if (v > u && !used[v] && desire[v] == u) {
                    used[u] = used[v] = true;
                    current_matching_indices.push_back(edge_to_idx[{u, v}]);
                }
            }
        }

        // Unidirectional pass
        for (int u = 1; u <= n; ++u) {
            if (!used[u] && desire[u] != 0) {
                int v = desire[u];
                if (!used[v]) {
                    used[u] = used[v] = true;
                    int u_small = std::min(u, v);
                    int v_large = std::max(u, v);
                    current_matching_indices.push_back(edge_to_idx[{u_small, v_large}]);
                }
            }
        }

        if (current_matching_indices.empty()) {
            break;
        }
        
        operations.push_back(current_matching_indices);
        
        std::map<int, std::pair<int, int>> idx_to_edge;
        for(auto const& [key, val] : edge_to_idx) {
            idx_to_edge[val] = key;
        }

        std::vector<std::pair<int, int>> swaps_to_perform;
        for (int edge_idx : current_matching_indices) {
            swaps_to_perform.push_back(idx_to_edge[edge_idx]);
        }
        
        for(auto const& edge : swaps_to_perform) {
            std::swap(p[edge.first], p[edge.second]);
        }
    }

    std::cout << operations.size() << "\n";
    for (const auto& op : operations) {
        std::cout << op.size();
        std::sort(op.begin(), op.end());
        for (int edge_idx : op) {
            std::cout << " " << edge_idx;
        }
        std::cout << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}