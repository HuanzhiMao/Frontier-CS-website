#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

void bfs(const vector<vector<int>>& g, const vector<int>& sources, vector<int>& dist, vector<int>& parent) {
    int n = g.size();
    dist.assign(n, INF);
    parent.assign(n, -1);
    queue<int> q;
    for (int s : sources) {
        dist[s] = 0;
        parent[s] = s;
        q.push(s);
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q.push(v);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<int> s(n), t(n);
    for (int i = 0; i < n; ++i) cin >> s[i];
    for (int i = 0; i < n; ++i) cin >> t[i];
    vector<vector<int>> g(n);
    for (int i = 0; i < m; ++i) {
        int u, v; cin >> u >> v;
        --u; --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    
    vector<int> S0, S1;
    for (int i = 0; i < n; ++i) {
        if (s[i] == 0) S0.push_back(i);
        else S1.push_back(i);
    }
    
    vector<int> dist0, parent0, dist1, parent1;
    bfs(g, S0, dist0, parent0);
    bfs(g, S1, dist1, parent1);
    
    vector<int> f(n), d(n);
    for (int i = 0; i < n; ++i) {
        if (t[i] == 0) {
            d[i] = dist0[i];
            f[i] = (d[i] == 0 ? i : parent0[i]);
        } else {
            d[i] = dist1[i];
            f[i] = (d[i] == 0 ? i : parent1[i]);
        }
    }
    
    int L = 0;
    for (int i = 0; i < n; ++i) {
        L = max(L, d[i]);
    }
    
    cout << L << "\n";
    vector<int> cur = s;
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << cur[i];
    }
    cout << "\n";
    for (int step = 1; step <= L; ++step) {
        vector<int> nxt(n);
        for (int i = 0; i < n; ++i) {
            nxt[i] = cur[f[i]];
        }
        for (int i = 0; i < n; ++i) {
            if (i) cout << ' ';
            cout << nxt[i];
        }
        cout << "\n";
        cur.swap(nxt);
    }
    
    return 0;
}