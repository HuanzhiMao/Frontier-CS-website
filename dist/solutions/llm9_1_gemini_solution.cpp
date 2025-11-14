#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <cstring>

// Using 1-based indexing for vertices
const int MAXN = 1005;
const int LOGN = 11;

std::vector<int> adj[MAXN];
int parent[MAXN];
int depth[MAXN];
int up[MAXN][LOGN];
int n;

void dfs(int u, int p, int d) {
    parent[u] = p;
    depth[u] = d;
    up[u][0] = p;
    for (int i = 1; i < LOGN; ++i) {
        if (up[u][i-1] != 0) {
            up[u][i] = up[up[u][i-1]][i-1];
        } else {
            up[u][i] = 0;
        }
    }
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u, d + 1);
        }
    }
}

int get_ancestor(int u, int k) {
    for (int i = LOGN - 1; i >= 0; --i) {
        if (k & (1 << i)) {
            if (u == 0) return 0;
            u = up[u][i];
        }
    }
    return u;
}

int lca(int u, int v) {
    if (depth[u] < depth[v]) std::swap(u, v);
    u = get_ancestor(u, depth[u] - depth[v]);
    if (u == v) return u;
    for (int i = LOGN - 1; i >= 0; --i) {
        if (up[u][i] != 0 && up[v][i] != 0 && up[u][i] != up[v][i]) {
            u = up[u][i];
            v = up[v][i];
        }
    }
    return parent[u];
}

int get_next_hop(int u, int target) {
    if (u == target) return u;
    int l = lca(u, target);
    if (l == u) {
        return get_ancestor(target, depth[target] - depth[u] - 1);
    } else {
        return parent[u];
    }
}


void solve() {
    std::cin >> n;
    std::vector<int> p(n + 1);
    bool sorted = true;
    for (int i = 1; i <= n; ++i) {
        std::cin >> p[i];
        if (p[i] != i) sorted = false;
    }

    for (int i = 1; i <= n; ++i) {
        adj[i].clear();
    }
    std::map<std::pair<int, int>, int> edge_to_idx;
    for (int i = 1; i < n; ++i) {
        int u, v;
        std::cin >> u >> v;
        if (u > v) std::swap(u, v);
        adj[u].push_back(v);
        adj[v].push_back(u);
        edge_to_idx[{u, v}] = i;
    }
    
    std::memset(up, 0, sizeof(up));
    dfs(1, 0, 0);

    std::vector<std::vector<int>> operations;

    while (!sorted) {
        std::map<int, int> desires;
        for (int i = 1; i <= n; ++i) {
            if (p[i] != i) {
                desires[i] = get_next_hop(i, p[i]);
            }
        }
        
        std::vector<std::pair<int, int>> current_matching_pairs;
        std::vector<bool> matched(n + 1, false);

        for (int u = 1; u <= n; ++u) {
            if (!matched[u] && p[u] != u) {
                int v = desires[u];
                if (!matched[v]) {
                    current_matching_pairs.push_back({u, v});
                    matched[u] = true;
                    matched[v] = true;
                }
            }
        }

        if (current_matching_pairs.empty()) {
            break; 
        }

        std::vector<int> current_op_edges;
        for (auto& edge : current_matching_pairs) {
            int u = edge.first;
            int v = edge.second;
            if (u > v) std::swap(u, v);
            current_op_edges.push_back(edge_to_idx.at({u, v}));
            
            std::swap(p[edge.first], p[edge.second]);
        }
        operations.push_back(current_op_edges);

        sorted = true;
        for (int i = 1; i <= n; ++i) {
            if (p[i] != i) {
                sorted = false;
                break;
            }
        }
    }

    std::cout << operations.size() << "\n";
    for (const auto& op : operations) {
        std::cout << op.size();
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