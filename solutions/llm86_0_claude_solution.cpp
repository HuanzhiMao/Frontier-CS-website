#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    vector<pair<int, int>> edges;
    
    // For each pair of nodes, find their LCA-like node
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            // Find a third node to query with
            int k = (i == 1 && j == 2) ? 3 : 1;
            if (k == i || k == j) k = (k == n) ? 1 : k + 1;
            while (k == i || k == j) k++;
            
            cout << "0 " << i << " " << j << " " << k << endl;
            cout.flush();
            
            int center;
            cin >> center;
            
            // The center is on the path between i and j
            // Try to determine if there's a direct edge
            bool direct_edge = true;
            
            // Check if i and j are directly connected by trying another query
            if (edges.size() < n - 1) {
                int l = 1;
                while (l == i || l == j || l == k) l++;
                
                cout << "0 " << i << " " << j << " " << l << endl;
                cout.flush();
                
                int center2;
                cin >> center2;
                
                // If both queries return the same center and it's either i or j,
                // then i and j are likely connected
                if (center == center2 && (center == i || center == j)) {
                    // Check if this edge would create a cycle
                    vector<bool> visited(n + 1, false);
                    bool creates_cycle = false;
                    
                    function<bool(int, int, int)> dfs = [&](int node, int target, int parent) -> bool {
                        if (node == target) return true;
                        visited[node] = true;
                        
                        for (int neighbor : adj[node]) {
                            if (neighbor != parent && !visited[neighbor]) {
                                if (dfs(neighbor, target, node)) return true;
                            }
                        }
                        return false;
                    };
                    
                    if (adj[i].size() > 0 || adj[j].size() > 0) {
                        creates_cycle = dfs(i, j, -1);
                    }
                    
                    if (!creates_cycle) {
                        adj[i].push_back(j);
                        adj[j].push_back(i);
                        edges.push_back({i, j});
                    }
                }
            }
            
            if (edges.size() == n - 1) break;
        }
        if (edges.size() == n - 1) break;
    }
    
    // If we don't have enough edges, use a simple approach
    while (edges.size() < n - 1) {
        for (int i = 1; i <= n && edges.size() < n - 1; i++) {
            for (int j = i + 1; j <= n && edges.size() < n - 1; j++) {
                // Check if already connected in our current graph
                vector<bool> visited(n + 1, false);
                function<bool(int, int)> connected = [&](int a, int b) -> bool {
                    if (a == b) return true;
                    visited[a] = true;
                    for (int neighbor : adj[a]) {
                        if (!visited[neighbor]) {
                            if (connected(neighbor, b)) return true;
                        }
                    }
                    return false;
                };
                
                if (!connected(i, j)) {
                    adj[i].push_back(j);
                    adj[j].push_back(i);
                    edges.push_back({i, j});
                }
            }
        }
    }
    
    cout << "1";
    for (auto& edge : edges) {
        cout << " " << edge.first << " " << edge.second;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}