#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

const int MAXN = 1005;
vector<pair<int, int>> adj[MAXN];
int p[MAXN];
int parent[MAXN];
int tin[MAXN], tout[MAXN];
int timer;
vector<pair<int, int>> edges_list;

void dfs_precompute(int u, int p_node) {
    parent[u] = p_node;
    tin[u] = ++timer;
    for (auto& edge : adj[u]) {
        int v = edge.first;
        if (v == p_node) continue;
        dfs_precompute(v, u);
    }
    tout[u] = ++timer;
}

bool is_in_subtree(int node_to_check, int subtree_root) {
    if (tin[subtree_root] == 0) return false;
    return tin[subtree_root] <= tin[node_to_check] && tout[node_to_check] <= tout[subtree_root];
}

bool is_sorted(int n) {
    for (int i = 1; i <= n; ++i) {
        if (p[i] != i) {
            return false;
        }
    }
    return true;
}

void solve() {
    int n;
    cin >> n;

    for (int i = 1; i <= n; ++i) {
        cin >> p[i];
        adj[i].clear();
    }
    edges_list.clear();
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back({v, i + 1});
        adj[v].push_back({u, i + 1});
        edges_list.push_back({u, v});
    }

    for (int i = 0; i <= n; ++i) {
        parent[i] = 0;
        tin[i] = 0;
        tout[i] = 0;
    }
    timer = 0;
    dfs_precompute(1, 0);

    vector<vector<int>> operations;

    while (!is_sorted(n)) {
        vector<pair<pair<int, int>, int>> type1_edges, type3_edges;
        
        for (int i = 0; i < n - 1; ++i) {
            int n1 = edges_list[i].first;
            int n2 = edges_list[i].second;
            int edge_idx = i + 1;

            int u, v; // u will be parent, v will be child
            if (parent[n1] == n2) {
                u = n2; v = n1;
            } else {
                u = n1; v = n2;
            }

            bool u_wants_down = is_in_subtree(p[u], v);
            bool v_wants_down = is_in_subtree(p[v], v);

            if (u_wants_down && !v_wants_down) {
                type1_edges.push_back({{n1, n2}, edge_idx});
            } else if (u_wants_down == v_wants_down) {
                type3_edges.push_back({{n1, n2}, edge_idx});
            }
        }

        vector<int> current_matching_indices;
        vector<bool> matched(n + 1, false);

        for (const auto& edge_info : type1_edges) {
            int u = edge_info.first.first;
            int v = edge_info.first.second;
            int idx = edge_info.second;
            if (!matched[u] && !matched[v]) {
                current_matching_indices.push_back(idx);
                matched[u] = matched[v] = true;
            }
        }
        for (const auto& edge_info : type3_edges) {
            int u = edge_info.first.first;
            int v = edge_info.first.second;
            int idx = edge_info.second;
            if (!matched[u] && !matched[v]) {
                current_matching_indices.push_back(idx);
                matched[u] = matched[v] = true;
            }
        }

        operations.push_back(current_matching_indices);

        for (int edge_idx : current_matching_indices) {
            int u = edges_list[edge_idx - 1].first;
            int v = edges_list[edge_idx - 1].second;
            swap(p[u], p[v]);
        }
    }

    cout << operations.size() << "\n";
    for (const auto& op : operations) {
        cout << op.size();
        for (int idx : op) {
            cout << " " << idx;
        }
        cout << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}