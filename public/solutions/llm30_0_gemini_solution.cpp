#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

vector<int> adj[5005];
int parent[5005];
int depth[5005];
int tin[5005], tout[5005];
int timer;
vector<int> post_order_nodes;

void dfs_precompute(int u, int p, int d) {
    parent[u] = p;
    depth[u] = d;
    tin[u] = ++timer;
    for (int v : adj[u]) {
        if (v != p) {
            dfs_precompute(v, u, d + 1);
        }
    }
    tout[u] = ++timer;
    post_order_nodes.push_back(u);
}

bool is_in_subtree(int u, int v) {
    // is v in subtree of u
    if (u == 0) return false;
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int query(int x) {
    cout << "? " << x << endl;
    int res;
    cin >> res;
    return res;
}

void answer(int x) {
    cout << "! " << x << endl;
}

void solve() {
    int n;
    cin >> n;

    for (int i = 1; i <= n; ++i) {
        adj[i].clear();
    }
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    timer = 0;
    post_order_nodes.clear();
    dfs_precompute(1, 0, 0);

    vector<int> candidates(n);
    iota(candidates.begin(), candidates.end(), 1);

    vector<int> subtree_C_size(n + 1);
    
    while (candidates.size() > 1) {
        fill(subtree_C_size.begin(), subtree_C_size.end(), 0);
        for (int node : candidates) {
            subtree_C_size[node] = 1;
        }
        for (int node : post_order_nodes) {
            if (parent[node] != 0) {
                subtree_C_size[parent[node]] += subtree_C_size[node];
            }
        }

        int centroid = 1;
        while (true) {
            int heavy_child = -1;
            for (int neighbor : adj[centroid]) {
                if (neighbor != parent[centroid]) {
                    if (subtree_C_size[neighbor] * 2 > (int)candidates.size()) {
                        heavy_child = neighbor;
                        break;
                    }
                }
            }
            if (heavy_child != -1) {
                centroid = heavy_child;
            } else {
                break;
            }
        }

        int query_node = centroid;
        if (subtree_C_size[centroid] == (int)candidates.size() && candidates.size() > 1) {
            int best_child = -1;
            int min_diff = candidates.size() + 1;

            for (int v : adj[centroid]) {
                if (v != parent[centroid]) {
                    int in_subtree = subtree_C_size[v];
                    int diff = abs((int)candidates.size() - 2 * in_subtree);
                    if (diff < min_diff) {
                        min_diff = diff;
                        best_child = v;
                    }
                }
            }
            query_node = best_child;
        }

        int res = query(query_node);

        vector<int> next_candidates;
        if (res == 1) {
            for (int node : candidates) {
                if (is_in_subtree(query_node, node)) {
                    next_candidates.push_back(node);
                }
            }
        } else {
            vector<bool> seen(n + 1, false);
            for (int node : candidates) {
                if (!is_in_subtree(query_node, node)) {
                    int p = (node == 1) ? 1 : parent[node];
                    if (!seen[p]) {
                        next_candidates.push_back(p);
                        seen[p] = true;
                    }
                }
            }
        }
        candidates = next_candidates;
    }

    answer(candidates[0]);
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