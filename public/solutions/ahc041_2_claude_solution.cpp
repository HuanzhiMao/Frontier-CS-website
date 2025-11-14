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
    
    for (int i = 0; i < N; i++) {
        int x, y;
        cin >> x >> y;
    }
    
    vector<int> parent(N, -1);
    vector<bool> visited(N, false);
    
    // Greedy approach: for each unvisited vertex, try to build a tree starting from it
    for (int start = 0; start < N; start++) {
        if (visited[start]) continue;
        
        // BFS to build tree with height limit H
        queue<pair<int, int>> q; // (vertex, height)
        q.push({start, 0});
        visited[start] = true;
        
        while (!q.empty()) {
            auto [u, h] = q.front();
            q.pop();
            
            if (h >= H) continue;
            
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
                    q.push({v, h + 1});
                }
            }
        }
    }
    
    for (int i = 0; i < N; i++) {
        cout << parent[i];
        if (i < N - 1) cout << " ";
    }
    cout << endl;
    
    return 0;
}