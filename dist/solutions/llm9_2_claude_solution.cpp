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
            cin >> i;
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
            vector<bool> fixed(n + 1, false);
            vector<int> current_matching;
            
            // Check which vertices are already correct
            for (int i = 1; i <= n; i++) {
                if (p[i] == i) {
                    fixed[i] = true;
                }
            }
            
            // Find a matching of edges where both endpoints need to be swapped
            vector<bool> used(n + 1, false);
            
            for (int i = 0; i < edges.size(); i++) {
                int u = edges[i].first;
                int v = edges[i].second;
                
                if (!used[u] && !used[v] && !fixed[u] && !fixed[v]) {
                    // Check if swapping would be beneficial
                    bool beneficial = false;
                    if (p[u] == v && p[v] == u) beneficial = true;
                    if (p[u] == v || p[v] == u) beneficial = true;
                    if (!beneficial && (p[u] != u || p[v] != v)) beneficial = true;
                    
                    if (beneficial) {
                        current_matching.push_back(i + 1);
                        used[u] = used[v] = true;
                    }
                }
            }
            
            if (current_matching.empty()) {
                // Try to find any beneficial swap
                for (int i = 0; i < edges.size(); i++) {
                    int u = edges[i].first;
                    int v = edges[i].second;
                    
                    if (!used[u] && !used[v] && (p[u] != u || p[v] != v)) {
                        current_matching.push_back(i + 1);
                        used[u] = used[v] = true;
                        break;
                    }
                }
            }
            
            if (current_matching.empty()) break;
            
            operations.push_back(current_matching);
            
            // Apply the swaps
            for (int idx : current_matching) {
                int u = edges[idx - 1].first;
                int v = edges[idx - 1].second;
                swap(p[u], p[v]);
            }
        }
        
        cout << operations.size() << "\n";
        for (auto& op : operations) {
            cout << op.size();
            for (int idx : op) {
                cout << " " << idx;
            }
            cout << "\n";
        }
    }
    
    return 0;
}