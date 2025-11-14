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
        vector<int> p(n + 1);
        for (int i = 1; i <= n; ++i) cin >> p[i];
        
        vector<vector<pair<int,int>>> adj(n + 1);
        vector<vector<int>> edgeId(n + 1, vector<int>(n + 1, -1));
        for (int i = 1; i <= n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back({v, i});
            adj[v].push_back({u, i});
            edgeId[u][v] = i;
            edgeId[v][u] = i;
        }
        
        vector<int> pos(n + 1);
        for (int i = 1; i <= n; ++i) pos[p[i]] = i;
        
        vector<int> deg(n + 1);
        vector<char> active(n + 1, 1);
        for (int i = 1; i <= n; ++i) deg[i] = (int)adj[i].size();
        
        queue<int> q;
        for (int i = 1; i <= n; ++i) if (deg[i] <= 1) q.push(i);
        
        vector<int> ops;
        int activeCnt = n;
        
        while (activeCnt > 0) {
            if (q.empty()) {
                for (int i = 1; i <= n; ++i) if (active[i] && deg[i] <= 1) q.push(i);
                if (q.empty()) break;
            }
            int v = q.front(); q.pop();
            if (!active[v]) continue;
            if (deg[v] == 0) {
                // Last remaining vertex
                active[v] = false;
                --activeCnt;
                continue;
            }
            if (deg[v] != 1) continue;
            
            if (p[v] != v) {
                int start = pos[v];
                if (start != v) {
                    vector<int> parent(n + 1, -1);
                    queue<int> bq;
                    parent[start] = start;
                    bq.push(start);
                    while (!bq.empty()) {
                        int x = bq.front(); bq.pop();
                        if (x == v) break;
                        for (auto &pr : adj[x]) {
                            int y = pr.first;
                            if (!active[y]) continue;
                            if (parent[y] != -1) continue;
                            parent[y] = x;
                            bq.push(y);
                        }
                    }
                    // Reconstruct path from start to v
                    vector<int> pathrev;
                    int cur = v;
                    while (cur != start) {
                        pathrev.push_back(cur);
                        cur = parent[cur];
                    }
                    pathrev.push_back(start);
                    for (int i = (int)pathrev.size() - 1; i > 0; --i) {
                        int a = pathrev[i];
                        int b = pathrev[i - 1];
                        int eidx = edgeId[a][b];
                        swap(p[a], p[b]);
                        pos[p[a]] = a;
                        pos[p[b]] = b;
                        ops.push_back(eidx);
                    }
                }
            }
            // Remove leaf v
            active[v] = false;
            --activeCnt;
            for (auto &e : adj[v]) {
                int w = e.first;
                if (active[w]) {
                    --deg[w];
                    if (deg[w] <= 1) q.push(w);
                }
            }
        }
        
        cout << ops.size() << '\n';
        for (int id : ops) {
            cout << 1 << ' ' << id << '\n';
        }
    }
    return 0;
}