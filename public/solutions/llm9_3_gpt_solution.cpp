#include <bits/stdc++.h>
using namespace std;

struct Edge { int u, v; };

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<int> p(n + 1);
        for (int i = 1; i <= n; ++i) cin >> p[i];

        vector<Edge> edges(n);
        vector<vector<pair<int,int>>> g(n + 1);
        for (int i = 1; i <= n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            edges[i] = {u, v};
            g[u].push_back({v, i});
            g[v].push_back({u, i});
        }

        // Root the tree at 1 to get parent, parentEdge, depth
        vector<int> parent(n + 1, 0), parentEdge(n + 1, 0), depth(n + 1, 0);
        vector<int> order;
        order.reserve(n);
        order.push_back(1);
        for (size_t it = 0; it < order.size(); ++it) {
            int u = order[it];
            for (auto [v, idx] : g[u]) {
                if (v == parent[u]) continue;
                parent[v] = u;
                parentEdge[v] = idx;
                depth[v] = depth[u] + 1;
                order.push_back(v);
            }
        }

        // Position of each token
        vector<int> pos(n + 1);
        for (int i = 1; i <= n; ++i) pos[p[i]] = i;

        vector<int> deg(n + 1);
        vector<char> active(n + 1, 1);
        deque<int> q;
        for (int i = 1; i <= n; ++i) {
            deg[i] = (int)g[i].size();
            if (deg[i] <= 1) q.push_back(i);
        }

        vector<int> ops;
        ops.reserve(n * 2); // rough reserve

        int processed = 0;
        while (processed < n) {
            if (q.empty()) {
                for (int i = 1; i <= n; ++i) if (active[i]) { q.push_back(i); break; }
                if (q.empty()) break;
            }
            int vtx = q.front(); q.pop_front();
            if (!active[vtx]) continue;

            int start = pos[vtx];
            if (start != vtx) {
                int u = start, v = vtx;
                vector<int> up, down;
                while (u != v) {
                    if (depth[u] > depth[v]) {
                        up.push_back(parentEdge[u]);
                        u = parent[u];
                    } else if (depth[v] > depth[u]) {
                        down.push_back(parentEdge[v]);
                        v = parent[v];
                    } else {
                        up.push_back(parentEdge[u]);
                        u = parent[u];
                        down.push_back(parentEdge[v]);
                        v = parent[v];
                    }
                }
                // perform swaps along path: up then reversed down
                for (int e : up) {
                    int a = edges[e].u, b = edges[e].v;
                    int pa = p[a], pb = p[b];
                    swap(p[a], p[b]);
                    pos[pa] = b;
                    pos[pb] = a;
                    ops.push_back(e);
                }
                for (int i = (int)down.size() - 1; i >= 0; --i) {
                    int e = down[i];
                    int a = edges[e].u, b = edges[e].v;
                    int pa = p[a], pb = p[b];
                    swap(p[a], p[b]);
                    pos[pa] = b;
                    pos[pb] = a;
                    ops.push_back(e);
                }
            }

            // remove vtx from active tree
            active[vtx] = 0;
            ++processed;
            for (auto [nei, idx] : g[vtx]) {
                if (active[nei]) {
                    deg[nei]--;
                    if (deg[nei] <= 1) q.push_back(nei);
                }
            }
            deg[vtx] = 0;
        }

        cout << ops.size() << '\n';
        for (int e : ops) {
            cout << 1 << ' ' << e << '\n';
        }
    }
    return 0;
}