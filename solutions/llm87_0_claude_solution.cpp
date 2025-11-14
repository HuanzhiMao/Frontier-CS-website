#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<int> initial(n), target(n);
    for (int i = 0; i < n; i++) {
        cin >> initial[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> target[i];
    }
    
    vector<vector<int>> adj(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    vector<vector<int>> steps;
    vector<int> current = initial;
    steps.push_back(current);
    
    while (current != target) {
        vector<int> next = current;
        
        for (int i = 0; i < n; i++) {
            if (current[i] != target[i]) {
                // Try to change node i to target color
                bool found = false;
                
                // Check if any neighbor has the target color
                for (int neighbor : adj[i]) {
                    if (current[neighbor] == target[i]) {
                        next[i] = target[i];
                        found = true;
                        break;
                    }
                }
                
                if (found) break;
            }
        }
        
        // If no direct change possible, find a node that can help propagate
        if (next == current) {
            for (int i = 0; i < n; i++) {
                if (current[i] == target[i]) continue;
                
                // Try to get a neighbor to have the target color for node i
                for (int neighbor : adj[i]) {
                    if (current[neighbor] == target[i]) continue;
                    
                    // Check if neighbor can get target color from its neighbors
                    for (int nn : adj[neighbor]) {
                        if (current[nn] == target[i]) {
                            next[neighbor] = target[i];
                            goto found_change;
                        }
                    }
                }
            }
            
            found_change:;
        }
        
        current = next;
        steps.push_back(current);
        
        if (steps.size() > 20000) break;
    }
    
    cout << steps.size() - 1 << "\n";
    for (const auto& step : steps) {
        for (int i = 0; i < n; i++) {
            cout << step[i];
            if (i < n - 1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}