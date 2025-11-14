#include <bits/stdc++.h>
using namespace std;

int main() {
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
    
    vector<pair<int, int>> pos(N);
    for (int i = 0; i < N; i++) {
        cin >> pos[i].first >> pos[i].second;
    }
    
    vector<int> parent(N, -1);
    vector<bool> visited(N, false);
    
    // Greedy approach: start from vertices with highest beauty values
    vector<pair<int, int>> beauty_order;
    for (int i = 0; i < N; i++) {
        beauty_order.push_back({A[i], i});
    }
    sort(beauty_order.rbegin(), beauty_order.rend());
    
    for (auto [beauty, root] : beauty_order) {
        if (visited[root]) continue;
        
        // BFS to build tree from this root
        queue<pair<int, int>> q; // {vertex, depth}
        q.push({root, 0});
        visited[root] = true;
        parent[root] = -1;
        
        while (!q.empty()) {
            auto [v, depth] = q.front();
            q.pop();
            
            if (depth >= H) continue;
            
            // Sort neighbors by beauty value (descending)
            vector<pair<int, int>> neighbors;
            for (int u : adj[v]) {
                if (!visited[u]) {
                    neighbors.push_back({A[u], u});
                }
            }
            sort(neighbors.rbegin(), neighbors.rend());
            
            for (auto [nb_beauty, u] : neighbors) {
                if (!visited[u]) {
                    visited[u] = true;
                    parent[u] = v;
                    q.push({u, depth + 1});
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