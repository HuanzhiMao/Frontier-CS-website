#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
#include <set>

void solve();

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

const int MAXN = 5005;
const int LOGN = 13;

std::vector<int> adj[MAXN];
int parent[MAXN];
int depth[MAXN];
int tin[MAXN], tout[MAXN];
int up[MAXN][LOGN];
int timer;
int n;

void dfs_lca(int v, int p, int d) {
    tin[v] = ++timer;
    parent[v] = p;
    depth[v] = d;
    up[v][0] = p;
    for (int i = 1; i < LOGN; ++i) {
        up[v][i] = up[up[v][i - 1]][i - 1];
    }

    for (int u : adj[v]) {
        if (u != p) {
            dfs_lca(u, v, d + 1);
        }
    }
    tout[v] = ++timer;
}

bool is_ancestor(int u, int v) {
    if (u == 0 || v == 0) return false;
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int i = LOGN - 1; i >= 0; --i) {
        if (!is_ancestor(up[u][i], v)) {
            u = up[u][i];
        }
    }
    return up[u][0];
}

void solve() {
    std::cin >> n;
    for (int i = 1; i <= n; ++i) {
        adj[i].clear();
    }
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    timer = 0;
    dfs_lca(1, 1, 0);

    std::vector<int> p(n);
    std::iota(p.begin(), p.end(), 1);

    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

    while (p.size() > 1) {
        std::set<int> candidates;

        if (p.size() <= 40) {
            for (int node : p) {
                candidates.insert(node);
            }
        } else {
            int min_tin_node = -1, max_tin_node = -1;
            int min_tin_val = 2 * n + 2, max_tin_val = -1;
            
            for(int node : p) {
                if (tin[node] < min_tin_val) {
                    min_tin_val = tin[node];
                    min_tin_node = node;
                }
                if (tin[node] > max_tin_val) {
                    max_tin_val = tin[node];
                    max_tin_node = node;
                }
            }
            if(min_tin_node != -1 && max_tin_node != -1)
                candidates.insert(lca(min_tin_node, max_tin_node));
            
            for (int i = 0; i < 20; ++i) {
                candidates.insert(p[std::uniform_int_distribution<int>(0, p.size() - 1)(rng)]);
            }
        }
        
        int best_x = -1;
        int max_gain = -1;
        int min_depth = n + 1;

        for (int x : candidates) {
            if (x == 0) continue;
            
            std::vector<int> p1_nodes;
            std::vector<int> p0_nodes_pre;
            for (int u : p) {
                if (is_ancestor(x, u)) {
                    p1_nodes.push_back(u);
                } else {
                    p0_nodes_pre.push_back(u);
                }
            }

            std::set<int> p0_parents;
            for(int u : p0_nodes_pre) {
                p0_parents.insert(parent[u]);
            }

            int p1_size = p1_nodes.size();
            int p0_size = p0_parents.size();
            
            int current_gain = p.size() - std::max(p1_size, p0_size);
            
            if (current_gain > max_gain) {
                max_gain = current_gain;
                min_depth = depth[x];
                best_x = x;
            } else if (current_gain == max_gain && depth[x] < min_depth) {
                min_depth = depth[x];
                best_x = x;
            }
        }
        
        if (best_x == -1) {
            best_x = p[0];
        }

        std::cout << "? " << best_x << std::endl;
        int response;
        std::cin >> response;

        std::vector<int> next_p;
        if (response == 1) {
            for (int u : p) {
                if (is_ancestor(best_x, u)) {
                    next_p.push_back(u);
                }
            }
        } else {
            std::set<int> next_p_set;
            for (int u : p) {
                if (!is_ancestor(best_x, u)) {
                    next_p_set.insert(parent[u]);
                }
            }
            for (int u : next_p_set) {
                next_p.push_back(u);
            }
        }
        p = next_p;
    }

    std::cout << "! " << p[0] << std::endl;
}