#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

using namespace std;

const int MAXN = 5005;
const int LOGN = 13;

vector<int> adj[MAXN];
int parent[MAXN];
int depth[MAXN];
int tin[MAXN], tout[MAXN];
int timer;
int up[MAXN][LOGN];

void dfs_lca(int v, int p, int d) {
    tin[v] = ++timer;
    parent[v] = p;
    depth[v] = d;
    up[v][0] = p;
    for (int i = 1; i < LOGN; ++i) {
        if (up[v][i - 1] != 0) {
            up[v][i] = up[up[v][i - 1]][i - 1];
        } else {
            up[v][i] = 0;
        }
    }

    for (int u : adj[v]) {
        if (u != p) {
            dfs_lca(u, v, d + 1);
        }
    }
    tout[v] = ++timer;
}

bool is_ancestor(int u, int v) {
    if (u == 0) return true;
    if (v == 0) return false;
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int i = LOGN - 1; i >= 0; --i) {
        if (up[u][i] != 0 && !is_ancestor(up[u][i], v)) {
            u = up[u][i];
        }
    }
    return up[u][0];
}

void solve() {
    int n;
    cin >> n;

    for (int i = 0; i <= n; ++i) {
        adj[i].clear();
        for(int j = 0; j < LOGN; ++j) {
            up[i][j] = 0;
        }
    }

    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    timer = 0;
    dfs_lca(1, 0, 0);

    vector<int> possible_nodes(n);
    iota(possible_nodes.begin(), possible_nodes.end(), 1);

    while (possible_nodes.size() > 1) {
        int best_node = -1;
        int min_max_split = n + 1;

        int common_ancestor = possible_nodes[0];
        for (size_t i = 1; i < possible_nodes.size(); ++i) {
            common_ancestor = lca(common_ancestor, possible_nodes[i]);
        }
        
        vector<int> candidates;
        if(common_ancestor != 0) {
            candidates.push_back(common_ancestor);
            for (int child : adj[common_ancestor]) {
                if (parent[common_ancestor] != child) {
                    candidates.push_back(child);
                }
            }
        } else { // Should not happen with nodes 1..n
            candidates.push_back(1);
        }


        for (int cand : candidates) {
            int in_subtree_count = 0;
            for (int node : possible_nodes) {
                if (is_ancestor(cand, node)) {
                    in_subtree_count++;
                }
            }
            int current_max_split = max(in_subtree_count, (int)possible_nodes.size() - in_subtree_count);
            if (current_max_split < min_max_split) {
                min_max_split = current_max_split;
                best_node = cand;
            } else if (current_max_split == min_max_split) {
                if (depth[cand] < depth[best_node]) {
                    best_node = cand;
                }
            }
        }
        
        cout << "? " << best_node << endl;
        int response;
        cin >> response;

        vector<int> next_possible_nodes;
        if (response == 1) {
            for (int node : possible_nodes) {
                if (is_ancestor(best_node, node)) {
                    next_possible_nodes.push_back(node);
                }
            }
        } else {
            for (int node : possible_nodes) {
                if (!is_ancestor(best_node, node)) {
                    if (node == 1) {
                        next_possible_nodes.push_back(1);
                    } else {
                        next_possible_nodes.push_back(parent[node]);
                    }
                }
            }
        }
        sort(next_possible_nodes.begin(), next_possible_nodes.end());
        next_possible_nodes.erase(unique(next_possible_nodes.begin(), next_possible_nodes.end()), next_possible_nodes.end());
        possible_nodes = next_possible_nodes;
    }

    cout << "! " << possible_nodes[0] << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.flush();
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}