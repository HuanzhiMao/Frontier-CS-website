#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<int> p(n + 1), pos(n + 1);
        for (int i = 1; i <= n; ++i) {
            cin >> p[i];
            pos[p[i]] = i;
        }
        vector<vector<pair<int,int>>> g(n + 1);
        vector<pair<int,int>> edges(n); // 1..n-1
        for (int i = 1; i <= n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            edges[i] = {u, v};
            g[u].push_back({v, i});
            g[v].push_back({u, i});
        }

        // LCA preprocessing
        int LOG = 1;
        while ((1 << LOG) <= n) ++LOG;
        vector<int> depth(n + 1, 0), parent(n + 1, 0);
        vector<vector<int>> up(LOG, vector<int>(n + 1, 0));

        // BFS to set parent and depth
        queue<int> q;
        int root = 1;
        parent[root] = 0;
        depth[root] = 0;
        q.push(root);
        vector<char> vis(n + 1, 0);
        vis[root] = 1;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : g[u]) {
                int v = e.first;
                if (vis[v]) continue;
                vis[v] = 1;
                parent[v] = u;
                depth[v] = depth[u] + 1;
                q.push(v);
            }
        }
        for (int v = 1; v <= n; ++v) up[0][v] = parent[v];
        for (int k = 1; k < LOG; ++k) {
            for (int v = 1; v <= n; ++v) {
                up[k][v] = up[k - 1][ up[k - 1][v] ];
            }
        }
        auto ascend = [&](int u, int k) {
            for (int i = 0; i < LOG; ++i) {
                if (k & (1 << i)) u = up[i][u];
            }
            return u;
        };
        auto lca = [&](int a, int b) {
            if (depth[a] < depth[b]) swap(a, b);
            int diff = depth[a] - depth[b];
            a = ascend(a, diff);
            if (a == b) return a;
            for (int i = LOG - 1; i >= 0; --i) {
                if (up[i][a] != up[i][b]) {
                    a = up[i][a];
                    b = up[i][b];
                }
            }
            return parent[a];
        };
        auto next_on_path = [&](int u, int v) {
            if (u == v) return u;
            int L = lca(u, v);
            if (L != u) return parent[u];
            int stepUp = depth[v] - depth[u] - 1;
            int w = ascend(v, stepUp);
            return w;
        };
        auto get_edge_id = [&](int u, int v) {
            if (g[u].size() <= g[v].size()) {
                for (auto &pr : g[u]) if (pr.first == v) return pr.second;
            } else {
                for (auto &pr : g[v]) if (pr.first == u) return pr.second;
            }
            return -1;
        };

        // Leaf-processing algorithm
        vector<int> deg(n + 1);
        vector<char> alive(n + 1, 1);
        for (int i = 1; i <= n; ++i) deg[i] = (int)g[i].size();

        queue<int> leaves;
        for (int i = 1; i <= n; ++i) if (deg[i] <= 1) leaves.push(i);

        int alive_cnt = n;
        vector<int> ops; ops.reserve(n * 10); // heuristic

        auto do_swap = [&](int u, int v) {
            int id = get_edge_id(u, v);
            ops.push_back(id);
            int a = p[u], b = p[v];
            swap(p[u], p[v]);
            pos[a] = v; pos[b] = u;
        };

        while (alive_cnt > 1) {
            int v = -1;
            while (!leaves.empty()) {
                int x = leaves.front(); leaves.pop();
                if (alive[x] && deg[x] == 1) { v = x; break; }
            }
            if (v == -1) {
                // Fallback scan (shouldn't generally happen)
                for (int i = 1; i <= n; ++i) {
                    if (alive[i] && deg[i] == 1) { v = i; break; }
                }
                if (v == -1) break;
            }

            int dest = v;
            int cur = pos[dest];

            while (cur != dest) {
                int w = next_on_path(cur, dest);
                do_swap(cur, w);
                cur = w;
            }

            // Remove leaf v
            alive[v] = 0;
            --alive_cnt;
            for (auto &pr : g[v]) {
                int u = pr.first;
                if (alive[u]) {
                    deg[u]--;
                    if (deg[u] == 1) leaves.push(u);
                }
            }
            deg[v] = 0;
        }

        cout << ops.size() << '\n';
        for (int id : ops) {
            cout << 1 << ' ' << id << '\n';
        }
    }
    return 0;
}