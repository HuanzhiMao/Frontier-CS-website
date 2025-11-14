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
    
    // Greedy approach: for each unvisited vertex, create a tree with high beauty values
    for (int start = 0; start < N; start++) {
        if (visited[start]) continue;
        
        // BFS to build tree from this root
        queue<pair<int, int>> q; // {vertex, depth}
        q.push({start, 0});
        visited[start] = true;
        
        while (!q.empty()) {
            auto [u, depth] = q.front();
            q.pop();
            
            if (depth >= H) continue;
            
            // Sort neighbors by beauty value in descending order
            vector<int> neighbors;
            for (int v : adj[u]) {
                if (!visited[v]) {
                    neighbors.push_back(v);
                }
            }
            
            sort(neighbors.begin(), neighbors.end(), [&](int a, int b) {
                return A[a] > A[b];
            });
            
            for (int v : neighbors) {
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push({v, depth + 1});
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