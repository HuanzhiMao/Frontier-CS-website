#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v;
    long long w;
    int idx;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M, K;
    if (!(cin >> N >> M >> K)) return 0;
    vector<int> x(N), y(N);
    for (int i = 0; i < N; i++) cin >> x[i] >> y[i];
    vector<Edge> edges(M);
    vector<vector<pair<int,int>>> adj(N); // (neighbor, edge index)
    for (int j = 0; j < M; j++) {
        int u, v; long long w;
        cin >> u >> v >> w;
        --u; --v;
        edges[j] = {u, v, w, j};
        adj[u].push_back({v, j});
        adj[v].push_back({u, j});
    }
    vector<int> a(K), b(K);
    for (int k = 0; k < K; k++) cin >> a[k] >> b[k];

    // Assign each resident to nearest station
    const long long INFLL = (1LL<<60);
    vector<long long> maxd2(N, -1);
    vector<int> cnt(N, 0);
    for (int k = 0; k < K; k++) {
        long long best = INFLL;
        int bi = -1;
        for (int i = 0; i < N; i++) {
            long long dx = (long long)x[i] - a[k];
            long long dy = (long long)y[i] - b[k];
            long long d2 = dx*dx + dy*dy;
            if (d2 < best) {
                best = d2;
                bi = i;
            }
        }
        if (bi != -1) {
            if (best > maxd2[bi]) maxd2[bi] = best;
            cnt[bi]++;
        }
    }

    auto ceil_sqrt = [&](long long z) -> int {
        if (z <= 0) return 0;
        long long y = sqrtl((long double)z);
        while ((y+1)*(y+1) <= z) y++;
        while (y*y > z) y--;
        if (y*y == z) return (int)min(5000LL, y);
        else return (int)min(5000LL, y+1);
    };

    vector<int> P(N, 0);
    vector<int> used_nodes;
    for (int i = 0; i < N; i++) {
        if (maxd2[i] >= 0) {
            P[i] = ceil_sqrt(maxd2[i]);
            if (P[i] > 0) used_nodes.push_back(i);
        } else {
            P[i] = 0;
        }
    }

    // Dijkstra from node 0 to build shortest path tree
    vector<long long> dist(N, INFLL);
    vector<int> parent(N, -1), parentEdge(N, -1);
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> pq;
    dist[0] = 0;
    pq.push({0,0});
    while (!pq.empty()) {
        auto [d,u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, ei] : adj[u]) {
            long long nd = d + edges[ei].w;
            if (nd < dist[v]) {
                dist[v] = nd;
                parent[v] = u;
                parentEdge[v] = ei;
                pq.push({nd, v});
            }
        }
    }

    // Select edges that connect used nodes to root along SPT
    vector<int> B(M, 0);
    for (int i : used_nodes) {
        int cur = i;
        while (cur != 0 && parentEdge[cur] != -1) {
            int ei = parentEdge[cur];
            B[ei] = 1;
            cur = parent[cur];
        }
    }

    // Output
    for (int i = 0; i < N; i++) {
        if (i) cout << ' ';
        cout << P[i];
    }
    cout << '\n';
    for (int j = 0; j < M; j++) {
        if (j) cout << ' ';
        cout << B[j];
    }
    cout << '\n';
    return 0;
}