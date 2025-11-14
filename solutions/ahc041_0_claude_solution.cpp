#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, H;
    cin >> N >> M >> H;
    
    vector<int> A(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }
    
    vector<vector<int>> adj(N);
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    vector<int> x(N), y(N);
    for (int i = 0; i < N; i++) {
        cin >> x[i] >> y[i];
    }
    
    vector<int> parent(N, -1);
    vector<bool> visited(N, false);
    
    // Try to build trees greedily
    for (int root = 0; root < N; root++) {
        if (visited[root]) continue;
        
        // BFS to build tree with height limit H
        queue<pair<int, int>> q; // {node, height}
        q.push({root, 0});
        visited[root] = true;
        
        while (!q.empty()) {
            auto [u, height] = q.front();
            q.pop();
            
            if (height >= H) continue;
            
            // Sort neighbors by beauty value in descending order
            vector<int> candidates;
            for (int v : adj[u]) {
                if (!visited[v]) {
                    candidates.push_back(v);
                }
            }
            
            sort(candidates.begin(), candidates.end(), [&](int a, int b) {
                return A[a] > A[b];
            });
            
            for (int v : candidates) {
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push({v, height + 1});
                }
            }
        }
    }
    
    for (int i = 0; i < N; i++) {
        cout << parent[i];
        if (i < N - 1) cout << " ";
    }
    cout << "\n";
    
    return 0;
}