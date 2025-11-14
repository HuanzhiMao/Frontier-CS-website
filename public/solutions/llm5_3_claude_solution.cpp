#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<int> a(10);
    for (int i = 0; i < 10; i++) {
        cin >> a[i];
    }
    
    vector<vector<int>> adj(n + 1);
    vector<int> indegree(n + 1, 0);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        indegree[v]++;
    }
    
    // Find topological ordering using Kahn's algorithm
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }
    
    vector<int> topo_order;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);
        
        for (int v : adj[u]) {
            indegree[v]--;
            if (indegree[v] == 0) {
                q.push(v);
            }
        }
    }
    
    // If we can get all vertices in topological order, it's a Hamiltonian path
    if (topo_order.size() == n) {
        // Verify it's actually a valid path
        bool valid = true;
        for (int i = 0; i < n - 1; i++) {
            bool found = false;
            for (int next : adj[topo_order[i]]) {
                if (next == topo_order[i + 1]) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                valid = false;
                break;
            }
        }
        
        if (valid) {
            cout << n << "\n";
            for (int i = 0; i < n; i++) {
                cout << topo_order[i];
                if (i < n - 1) cout << " ";
            }
            cout << "\n";
            return 0;
        }
    }
    
    // Find longest path using DFS from each vertex
    vector<int> best_path;
    
    for (int start = 1; start <= n; start++) {
        vector<bool> visited(n + 1, false);
        vector<int> path;
        
        function<void(int)> dfs = [&](int u) {
            visited[u] = true;
            path.push_back(u);
            
            if (path.size() > best_path.size()) {
                best_path = path;
            }
            
            for (int v : adj[u]) {
                if (!visited[v]) {
                    dfs(v);
                }
            }
            
            path.pop_back();
            visited[u] = false;
        };
        
        dfs(start);
    }
    
    cout << best_path.size() << "\n";
    for (int i = 0; i < best_path.size(); i++) {
        cout << best_path[i];
        if (i < best_path.size() - 1) cout << " ";
    }
    cout << "\n";
    
    return 0;
}