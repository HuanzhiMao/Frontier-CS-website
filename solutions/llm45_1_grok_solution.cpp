#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m, k;
    double eps;
    cin >> n >> m >> k >> eps;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        if (u == v) continue;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) {
        sort(adj[i].begin(), adj[i].end());
        auto it = unique(adj[i].begin(), adj[i].end());
        adj[i].erase(it, adj[i].end());
    }
    srand(time(0));
    int start = (rand() % n) + 1;
    auto bfs_dist = [&](int s) -> vector<int> {
        vector<int> dist(n + 1, -1);
        queue<int> q;
        q.push(s);
        dist[s] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        return dist;
    };
    auto d1 = bfs_dist(start);
    int maxd = -1, far = 0;
    for (int i = 1; i <= n; i++) {
        if (d1[i] > maxd) {
            maxd = d1[i];
            far = i;
        }
    }
    if (far == 0) far = 1;
    auto d2 = bfs_dist(far);
    int maxd2 = -1, far2 = 0;
    for (int i = 1; i <= n; i++) {
        if (d2[i] > maxd2) {
            maxd2 = d2[i];
            far2 = i;
        }
    }
    if (far2 == 0) far2 = 1;
    vector<int> order;
    vector<bool> visited(n + 1, false);
    queue<int> q;
    q.push(far2);
    visited[far2] = true;
    order.push_back(far2);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                order.push_back(v);
                q.push(v);
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            q.push(i);
            visited[i] = true;
            order.push_back(i);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        order.push_back(v);
                        q.push(v);
                    }
                }
            }
        }
    }
    int base = n / k;
    int rem = n % k;
    vector<int> part_size(k);
    for (int i = 0; i < k; i++) {
        part_size[i] = base + (i < rem);
    }
    vector<int> p(n + 1);
    int idx = 0;
    for (int part = 1; part <= k; part++) {
        for (int j = 0; j < part_size[part - 1]; j++) {
            int v = order[idx++];
            p[v] = part;
        }
    }
    for (int i = 1; i <= n; i++) {
        if (i > 1) cout << " ";
        cout << p[i];
    }
    cout << "\n";
    return 0;
}