#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

const int MAXN = 5005;
const int LOGN = 13;

int n;
vector<int> adj[MAXN];
int parent[MAXN];
int depth[MAXN];
int tin[MAXN], tout[MAXN];
int timer;
int up[MAXN][LOGN + 1];
vector<int> post_order;

void dfs_precompute(int v, int p, int d) {
    tin[v] = ++timer;
    parent[v] = p;
    depth[v] = d;
    up[v][0] = p;
    for (int i = 1; i <= LOGN; ++i) {
        if (up[v][i - 1] != 0) {
            up[v][i] = up[up[v][i - 1]][i - 1];
        } else {
            up[v][i] = 0;
        }
    }
    for (int u : adj[v]) {
        if (u != p) {
            dfs_precompute(u, v, d + 1);
        }
    }
    tout[v] = ++timer;
    post_order.push_back(v);
}

bool is_ancestor(int u, int v) {
    if (u == 0) return true;
    if (v == 0) return false;
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int i = LOGN; i >= 0; --i) {
        if (up[u][i] != 0 && !is_ancestor(up[u][i], v)) {
            u = up[u][i];
        }
    }
    return up[u][0];
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
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        adj[i].clear();
        for (int j = 0; j <= LOGN; ++j) {
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
    post_order.clear();
    dfs_precompute(1, 0, 0);

    vector<int> P(n);
    iota(P.begin(), P.end(), 1);

    while (P.size() > 1) {
        vector<int> count_sub(n + 1, 0);
        for (int p : P) {
            count_sub[p] = 1;
        }

        for (int v : post_order) {
            if (parent[v] != 0) {
                count_sub[parent[v]] += count_sub[v];
            }
        }
        
        int min_tin_node = -1, max_tin_node = -1;
        int min_tin_val = n * 2 + 2, max_tin_val = -1;
        for (int p : P) {
            if (tin[p] < min_tin_val) {
                min_tin_val = tin[p];
                min_tin_node = p;
            }
            if (tin[p] > max_tin_val) {
                max_tin_val = tin[p];
                max_tin_node = p;
            }
        }
        
        int R = lca(min_tin_node, max_tin_node);
        
        int centroid = R;
        while(true) {
            int heavy_child = -1;
            int max_size = -1;
            for (int child : adj[centroid]) {
                if (child != parent[centroid] && count_sub[child] > max_size) {
                    max_size = count_sub[child];
                    heavy_child = child;
                }
            }
            if (heavy_child != -1 && count_sub[heavy_child] * 2 > P.size()) {
                centroid = heavy_child;
            } else {
                break;
            }
        }
        
        int c = centroid;
        int res = query(c);
        
        vector<int> next_P;
        if (res == 1) {
            for (int p : P) {
                if (is_ancestor(c, p)) {
                    next_P.push_back(p);
                }
            }
        } else {
            vector<bool> moved(n + 1, false);
            for (int p : P) {
                if (!is_ancestor(c, p)) {
                    int par = (p == 1) ? 1 : parent[p];
                    if (!moved[par]) {
                        next_P.push_back(par);
                        moved[par] = true;
                    }
                }
            }
        }
        P = next_P;
    }

    answer(P[0]);
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