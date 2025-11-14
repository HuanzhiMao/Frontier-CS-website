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
        vector<int> p(n+1), pos(n+1);
        for (int i = 1; i <= n; ++i) {
            cin >> p[i];
            pos[p[i]] = i;
        }
        vector<int> U(n), V(n);
        vector<vector<pair<int,int>>> g(n+1);
        for (int i = 1; i <= n-1; ++i) {
            int u, v;
            cin >> u >> v;
            U[i] = u; V[i] = v;
            g[u].push_back({v, i});
            g[v].push_back({u, i});
        }
        
        vector<char> active(n+1, 1);
        vector<int> deg(n+1, 0);
        for (int i = 1; i <= n; ++i) deg[i] = (int)g[i].size();
        int activeCount = n;
        queue<int> q;
        for (int i = 1; i <= n; ++i) if (deg[i] <= 1) q.push(i);
        
        vector<int> ops;
        ops.reserve(n*n/2 + 10);
        
        auto swapEdge = [&](int eid){
            int a = U[eid], b = V[eid];
            int ta = p[a], tb = p[b];
            p[a] = tb; p[b] = ta;
            pos[ta] = b; pos[tb] = a;
            ops.push_back(eid);
        };
        
        vector<int> parent, parentEdge;
        while (activeCount > 1) {
            while (!q.empty() && (!active[q.front()] || deg[q.front()] != 1)) q.pop();
            if (q.empty()) break; // should not happen in a tree
            int vtx = q.front(); q.pop();
            if (!active[vtx] || deg[vtx] != 1) continue;
            
            int start = pos[vtx];
            if (start != vtx) {
                parent.assign(n+1, -1);
                parentEdge.assign(n+1, -1);
                queue<int> qb;
                parent[start] = -2;
                qb.push(start);
                while (!qb.empty() && parent[vtx] == -1) {
                    int u = qb.front(); qb.pop();
                    for (auto [nb, eid] : g[u]) {
                        if (!active[nb]) continue;
                        if (parent[nb] != -1) continue;
                        parent[nb] = u;
                        parentEdge[nb] = eid;
                        qb.push(nb);
                        if (nb == vtx) break;
                    }
                }
                // reconstruct path from start to vtx
                vector<int> path;
                int cur = vtx;
                while (cur != start) {
                    path.push_back(parentEdge[cur]);
                    cur = parent[cur];
                }
                reverse(path.begin(), path.end());
                for (int e : path) swapEdge(e);
            }
            // deactivate vtx
            active[vtx] = 0;
            --activeCount;
            for (auto [nb, eid] : g[vtx]) {
                if (active[nb]) {
                    --deg[nb];
                    if (deg[nb] == 1) q.push(nb);
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