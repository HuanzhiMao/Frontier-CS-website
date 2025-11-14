#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<int> p(n + 1);
        for (int i = 1; i <= n; ++i) cin >> p[i];

        vector<Edge> edges(n); // 1..n-1 used
        vector<vector<pair<int,int>>> g(n + 1);
        vector<vector<int>> idmat(n + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            edges[i] = {u, v};
            g[u].push_back({v, i});
            g[v].push_back({u, i});
            idmat[u][v] = idmat[v][u] = i;
        }

        // LCA preprocessing
        int LOG = 20;
        vector<int> depth(n + 1, 0), parent(n + 1, 0), tin(n + 1, 0), tout(n + 1, 0);
        vector<vector<int>> up(LOG, vector<int>(n + 1, 0));
        int timer = 0;

        function<void(int,int)> dfs = [&](int u, int p) {
            parent[u] = p;
            up[0][u] = (p == 0 ? u : p);
            for (int j = 1; j < LOG; ++j) up[j][u] = up[j - 1][ up[j - 1][u] ];
            tin[u] = ++timer;
            for (auto [v, id] : g[u]) if (v != p) {
                depth[v] = depth[u] + 1;
                dfs(v, u);
            }
            tout[u] = ++timer;
        };

        dfs(1, 0);

        auto isAncestor = [&](int u, int v) -> bool {
            return tin[u] <= tin[v] && tout[u] >= tout[v];
        };

        function<int(int,int)> lca = [&](int a, int b) -> int {
            if (isAncestor(a, b)) return a;
            if (isAncestor(b, a)) return b;
            for (int j = LOG - 1; j >= 0; --j) {
                int aa = up[j][a];
                if (!isAncestor(aa, b)) a = aa;
            }
            return up[0][a];
        };

        auto move_up = [&](int v, int k) -> int {
            for (int j = 0; j < LOG; ++j) if (k & (1 << j)) v = up[j][v];
            return v;
        };

        // Token positions
        vector<int> valAt(n + 1), posOf(n + 1);
        for (int i = 1; i <= n; ++i) {
            valAt[i] = p[i];
            posOf[p[i]] = i;
        }
        int bad = 0;
        for (int i = 1; i <= n; ++i) if (valAt[i] != i) ++bad;

        // order by depth descending for selecting upward moves
        vector<int> orderDesc(n);
        iota(orderDesc.begin(), orderDesc.end(), 1);
        sort(orderDesc.begin(), orderDesc.end(), [&](int a, int b){
            return depth[a] > depth[b];
        });

        vector<vector<int>> ops;

        while (bad > 0) {
            vector<int> nxt(n + 1, -1);
            for (int u = 1; u <= n; ++u) {
                int t = valAt[u];
                if (t == u) continue;
                int L = lca(u, t);
                if (L == u) {
                    int steps = depth[t] - depth[u] - 1;
                    int c = (steps >= 0 ? move_up(t, steps) : t);
                    nxt[u] = c;
                } else {
                    nxt[u] = parent[u];
                }
            }

            vector<int> used(n + 1, 0);
            vector<int> op;

            // Step A: mutual edges
            for (int u = 1; u <= n; ++u) {
                int v = nxt[u];
                if (v == -1) continue;
                if (nxt[v] == u && !used[u] && !used[v]) {
                    used[u] = used[v] = 1;
                    op.push_back(idmat[u][v]);
                }
            }

            // Step B: add upward edges (child -> parent), prefer deeper nodes
            for (int u : orderDesc) {
                int v = nxt[u];
                if (v == -1) continue;
                if (v == parent[u] && v != 0 && !used[u] && !used[v]) {
                    used[u] = used[v] = 1;
                    op.push_back(idmat[u][v]);
                }
            }

            // Step C: fallback if still empty, pick any desired edge
            if (op.empty()) {
                for (int u = 1; u <= n; ++u) {
                    int v = nxt[u];
                    if (v == -1) continue;
                    if (!used[u] && !used[v]) {
                        used[u] = used[v] = 1;
                        op.push_back(idmat[u][v]);
                        break;
                    }
                }
            }

            // Apply operation
            for (int id : op) {
                int x = edges[id].u;
                int y = edges[id].v;
                int ax = valAt[x];
                int ay = valAt[y];
                bad -= (ax == x) + (ay == y);
                swap(valAt[x], valAt[y]);
                posOf[ax] = y;
                posOf[ay] = x;
                bad += (valAt[x] == x) + (valAt[y] == y);
            }

            ops.push_back(move(op));
        }

        cout << ops.size() << "\n";
        for (auto &op : ops) {
            cout << op.size();
            for (int id : op) cout << " " << id;
            cout << "\n";
        }
    }
    return 0;
}