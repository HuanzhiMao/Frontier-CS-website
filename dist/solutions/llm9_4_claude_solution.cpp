#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        int n;
        cin >> n;
        
        vector<int> p(n + 1);
        for (int i = 1; i <= n; i++) {
            cin >> i >> p[i];
        }
        
        vector<vector<int>> adj(n + 1);
        vector<pair<int, int>> edges;
        
        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
            edges.push_back({u, v});
        }
        
        vector<vector<int>> operations;
        
        while (true) {
            bool changed = false;
            vector<int> matching;
            vector<bool> used(n + 1, false);
            
            for (int i = 0; i < edges.size(); i++) {
                int u = edges[i].first;
                int v = edges[i].second;
                
                if (!used[u] && !used[v] && p[u] != u && p[v] != v) {
                    bool beneficial = false;
                    
                    // Check if swapping helps
                    if (p[u] == v || p[v] == u) {
                        beneficial = true;
                    }
                    
                    if (beneficial) {
                        matching.push_back(i + 1);
                        used[u] = used[v] = true;
                        swap(p[u], p[v]);
                        changed = true;
                    }
                }
            }
            
            // If no beneficial swaps found, try any swap that doesn't make things worse
            if (!changed) {
                for (int i = 0; i < edges.size(); i++) {
                    int u = edges[i].first;
                    int v = edges[i].second;
                    
                    if (!used[u] && !used[v] && (p[u] != u || p[v] != v)) {
                        matching.push_back(i + 1);
                        used[u] = used[v] = true;
                        swap(p[u], p[v]);
                        changed = true;
                        break;
                    }
                }
            }
            
            if (!changed) break;
            
            operations.push_back(matching);
            
            // Check if solved
            bool solved = true;
            for (int i = 1; i <= n; i++) {
                if (p[i] != i) {
                    solved = false;
                    break;
                }
            }
            if (solved) break;
        }
        
        cout << operations.size() << "\n";
        for (auto& op : operations) {
            cout << op.size();
            for (int edge : op) {
                cout << " " << edge;
            }
            cout << "\n";
        }
    }
    
    return 0;
}