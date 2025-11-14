#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M, H;
    cin >> N >> M >> H;
    
    vector<int> A(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }
    
    vector<vector<int>> graph(N);
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    
    for (int i = 0; i < N; i++) {
        int x, y;
        cin >> x >> y;
    }
    
    vector<int> parent(N, -1);
    vector<bool> visited(N, false);
    
    // Greedy approach: for each unvisited node, build a tree starting from it
    for (int start = 0; start < N; start++) {
        if (visited[start]) continue;
        
        // BFS to build tree from this start node
        queue<pair<int, int>> q; // {node, depth}
        q.push({start, 0});
        visited[start] = true;
        
        while (!q.empty()) {
            auto [node, depth] = q.front();
            q.pop();
            
            if (depth >= H) continue;
            
            // Sort neighbors by beauty value in descending order
            vector<int> neighbors;
            for (int neighbor : graph[node]) {
                if (!visited[neighbor]) {
                    neighbors.push_back(neighbor);
                }
            }
            
            sort(neighbors.begin(), neighbors.end(), [&](int a, int b) {
                return A[a] > A[b];
            });
            
            for (int neighbor : neighbors) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = node;
                    q.push({neighbor, depth + 1});
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