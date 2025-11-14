#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        int n;
        cin >> n;
        
        vector<vector<int>> adj(n + 1);
        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        
        vector<int> values(n + 1);
        
        // Query f(1)
        cout << "? 1 1 1" << endl;
        cin >> values[1];
        
        // For each other node, find its value
        vector<bool> visited(n + 1, false);
        queue<int> q;
        q.push(1);
        visited[1] = true;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                    
                    // Query f(v)
                    cout << "? 1 1 " << v << endl;
                    int fv;
                    cin >> fv;
                    
                    values[v] = fv - values[u];
                }
            }
        }
        
        cout << "!";
        for (int i = 1; i <= n; i++) {
            cout << " " << values[i];
        }
        cout << endl;
    }
    
    return 0;
}