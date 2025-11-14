#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <tuple>

using namespace std;

const int MAXN = 100005;
const int LOGN = 18;

int n;
vector<tuple<int, int, int>> edges;
int parent[MAXN];
int depth[MAXN];
long long dist_from_root[MAXN];
int up[MAXN][LOGN];

long long query(int u, int v) {
    cout << "? " << u << " " << v << endl;
    long long d;
    cin >> d;
    return d;
}

void answer() {
    cout << "!";
    for (const auto& edge : edges) {
        cout << " " << get<0>(edge) << " " << get<1>(edge) << " " << get<2>(edge);
    }
    cout << endl;
}

void add_node_data(int u, int p, int w) {
    parent[u] = p;
    depth[u] = (p == 0) ? 0 : depth[p] + 1;
    dist_from_root[u] = (p == 0) ? 0 : dist_from_root[p] + w;
    
    if (p != 0) {
        edges.emplace_back(u, p, w);
    }

    up[u][0] = p;
    for (int j = 1; j < LOGN; ++j) {
        if (up[u][j-1] != 0) {
            up[u][j] = up[up[u][j-1]][j-1];
        } else {
            up[u][j] = 0;
        }
    }
}

int lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    
    int k = depth[u] - depth[v];
    for (int j = LOGN - 1; j >= 0; --j) {
        if ((k >> j) & 1) {
            u = up[u][j];
        }
    }

    if (u == v) return u;

    for (int j = LOGN - 1; j >= 0; --j) {
        if (up[u][j] != 0 && up[v][j] != 0 && up[u][j] != up[v][j]) {
            u = up[u][j];
            v = up[v][j];
        }
    }
    return parent[u];
}

long long get_dist(int u, int v) {
    int l = lca(u, v);
    return dist_from_root[u] + dist_from_root[v] - 2 * dist_from_root[l];
}

int get_ancestor_at_dist(int u, long long dist_from_u) {
    long long target_dist_from_root = dist_from_root[u] - dist_from_u;
    int curr = u;
    for (int j = LOGN - 1; j >= 0; --j) {
        if (up[curr][j] != 0 && dist_from_root[up[curr][j]] >= target_dist_from_root) {
            curr = up[curr][j];
        }
    }
    return curr;
}

void solve() {
    cin >> n;

    for (int i = 0; i <= n; ++i) {
        parent[i] = 0;
        depth[i] = 0;
        dist_from_root[i] = 0;
        for (int j = 0; j < LOGN; ++j) {
            up[i][j] = 0;
        }
    }
    edges.clear();

    if (n == 1) {
        answer();
        return;
    }

    add_node_data(1, 0, 0);

    long long w12 = query(1, 2);
    add_node_data(2, 1, w12);

    int d1 = 1, d2 = 2;
    long long diam_dist = w12;

    for (int i = 3; i <= n; ++i) {
        long long d_d1_i = query(d1, i);
        long long d_d2_i = query(d2, i);

        long long dist_d1_attach = (diam_dist + d_d1_i - d_d2_i) / 2;
        
        int l = lca(d1, d2);
        long long dist_d1_l = dist_from_root[d1] - dist_from_root[l];

        int u_attach;
        if (dist_d1_attach <= dist_d1_l) {
            u_attach = get_ancestor_at_dist(d1, dist_d1_attach);
        } else {
            long long dist_d2_attach = diam_dist - dist_d1_attach;
            u_attach = get_ancestor_at_dist(d2, dist_d2_attach);
        }
        
        long long w_i_attach = d_d1_i - get_dist(d1, u_attach);
        add_node_data(i, u_attach, w_i_attach);

        long long dist_d1_new = get_dist(d1, i);
        long long dist_d2_new = get_dist(d2, i);
        
        if (dist_d1_new > diam_dist && dist_d1_new >= dist_d2_new) {
            diam_dist = dist_d1_new;
            d2 = i;
        } else if (dist_d2_new > diam_dist) {
            diam_dist = dist_d2_new;
            d1 = i;
        }
    }
    
    answer();
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    cin >> T;
    while (T--) {
        solve();
    }

    return 0;
}