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
            cin >> p[i];
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
            bool found = false;
            vector<int> matching;
            vector<bool> used(n + 1, false);
            
            // Try to find swaps that bring elements closer to their target
            for (int i = 0; i < edges.size(); i++) {
                int u = edges[i].first;
                int v = edges[i].second;
                
                if (used[u] || used[v]) continue;
                
                // Check if swapping would be beneficial
                bool beneficial = false;
                if (p[u] == v || p[v] == u) {
                    beneficial = true;
                } else if ((p[u] != u && p[v] != v)) {
                    // Check if this swap brings us closer to solution
                    int dist_before = 0, dist_after = 0;
                    if (p[u] != u) dist_before++;
                    if (p[v] != v) dist_before++;
                    if (p[v] != u) dist_after++;
                    if (p[u] != v) dist_after++;
                    if (dist_after <= dist_before) {
                        beneficial = true;
                    }
                }
                
                if (beneficial) {
                    matching.push_back(i);
                    used[u] = used[v] = true;
                    found = true;
                }
            }
            
            if (!found) {
                // If no beneficial swaps found, try any swap that's not in correct position
                for (int i = 0; i < edges.size(); i++) {
                    int u = edges[i].first;
                    int v = edges[i].second;
                    
                    if (used[u] || used[v]) continue;
                    
                    if (p[u] != u || p[v] != v) {
                        matching.push_back(i);
                        used[u] = used[v] = true;
                        found = true;
                        break;
                    }
                }
            }
            
            if (!found) break;
            
            // Apply the matching
            for (int idx : matching) {
                int u = edges[idx].first;
                int v = edges[idx].second;
                swap(p[u], p[v]);
            }
            
            operations.push_back(matching);
        }
        
        cout << operations.size() << "\n";
        for (auto& op : operations) {
            cout << op.size();
            for (int idx : op) {
                cout << " " << (idx + 1);
            }
            cout << "\n";
        }
    }
    
    return 0;
}