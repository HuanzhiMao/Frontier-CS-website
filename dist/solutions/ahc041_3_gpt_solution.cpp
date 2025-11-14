#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, H;
    if (!(cin >> N >> M >> H)) return 0;
    vector<int> A(N);
    for (int i = 0; i < N; ++i) cin >> A[i];
    vector<vector<int>> adj(N);
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    // Read coordinates (unused)
    for (int i = 0; i < N; ++i) {
        int x, y;
        cin >> x >> y;
    }

    // Order vertices by (A, idx) descending
    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b){
        if (A[a] != A[b]) return A[a] > A[b];
        return a > b;
    });

    auto higher = [&](int u, int v) {
        if (A[u] != A[v]) return A[u] > A[v];
        return u > v;
    };

    vector<int> parent(N, -1);
    vector<int> depth(N, 0);

    for (int v : order) {
        int best_u = -1;
        int best_d = -1;
        for (int u : adj[v]) {
            if (!higher(u, v)) continue;           // parent must have higher key
            if (depth[u] > H - 1) continue;        // ensure depth constraint after attaching
            if (depth[u] > best_d) {
                best_d = depth[u];
                best_u = u;
            } else if (depth[u] == best_d && best_u != -1) {
                // tie-break by higher beauty, then by index
                if (A[u] > A[best_u] || (A[u] == A[best_u] && u > best_u)) {
                    best_u = u;
                }
            }
        }
        if (best_u != -1) {
            parent[v] = best_u;
            depth[v] = depth[best_u] + 1;
        } else {
            parent[v] = -1;
            depth[v] = 0;
        }
    }

    for (int i = 0; i < N; ++i) {
        if (i) cout << ' ';
        cout << parent[i];
    }
    cout << '\n';
    return 0;
}