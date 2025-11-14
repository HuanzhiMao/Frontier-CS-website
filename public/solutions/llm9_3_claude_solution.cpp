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
            bool changed = false;
            vector<bool> used(n + 1, false);
            vector<int> current_op;
            
            for (int i = 0; i < edges.size(); i++) {
                int u = edges[i].first;
                int v = edges[i].second;
                
                if (!used[u] && !used[v] && p[u] != u && p[v] != v) {
                    // Check if swapping improves the situation
                    int score_before = 0;
                    int score_after = 0;
                    
                    if (p[u] == u) score_before++;
                    if (p[v] == v) score_before++;
                    if (p[v] == u) score_after++;
                    if (p[u] == v) score_after++;
                    
                    if (score_after > score_before) {
                        current_op.push_back(i + 1);
                        used[u] = true;
                        used[v] = true;
                        swap(p[u], p[v]);
                        changed = true;
                    }
                }
            }
            
            if (!changed) {
                // Try any beneficial swap
                for (int i = 0; i < edges.size(); i++) {
                    int u = edges[i].first;
                    int v = edges[i].second;
                    
                    if (!used[u] && !used[v] && (p[u] != u || p[v] != v)) {
                        int score_before = 0;
                        int score_after = 0;
                        
                        if (p[u] == u) score_before++;
                        if (p[v] == v) score_before++;
                        if (p[v] == u) score_after++;
                        if (p[u] == v) score_after++;
                        
                        if (score_after >= score_before) {
                            current_op.push_back(i + 1);
                            used[u] = true;
                            used[v] = true;
                            swap(p[u], p[v]);
                            changed = true;
                        }
                    }
                }
            }
            
            if (!changed) break;
            
            if (!current_op.empty()) {
                operations.push_back(current_op);
            }
        }
        
        cout << operations.size() << "\n";
        for (auto& op : operations) {
            cout << op.size();
            for (int edge_idx : op) {
                cout << " " << edge_idx;
            }
            cout << "\n";
        }
    }
    
    return 0;
}