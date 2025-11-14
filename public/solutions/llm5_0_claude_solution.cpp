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
    
    // Try topological sort approach
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }
    
    vector<int> topo_order;
    vector<int> temp_indegree = indegree;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);
        
        for (int v : adj[u]) {
            temp_indegree[v]--;
            if (temp_indegree[v] == 0) {
                q.push(v);
            }
        }
    }
    
    if (topo_order.size() == n) {
        // Check if this topological order gives a Hamiltonian path
        bool valid = true;
        for (int i = 0; i < n - 1; i++) {
            int u = topo_order[i];
            int v = topo_order[i + 1];
            bool found = false;
            for (int next : adj[u]) {
                if (next == v) {
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
    
    // If topological sort doesn't work, use DFS to find longest path
    vector<bool> visited(n + 1, false);
    vector<int> best_path;
    
    function<void(int, vector<int>&)> dfs = [&](int u, vector<int>& path) {
        visited[u] = true;
        path.push_back(u);
        
        if (path.size() > best_path.size()) {
            best_path = path;
        }
        
        for (int v : adj[u]) {
            if (!visited[v]) {
                dfs(v, path);
            }
        }
        
        path.pop_back();
        visited[u] = false;
    };
    
    for (int start = 1; start <= n; start++) {
        vector<int> path;
        dfs(start, path);
        if (best_path.size() == n) break;
    }
    
    cout << best_path.size() << "\n";
    for (int i = 0; i < best_path.size(); i++) {
        cout << best_path[i];
        if (i < best_path.size() - 1) cout << " ";
    }
    cout << "\n";
    
    return 0;
}