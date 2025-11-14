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
    
    // Simple greedy approach: for each incorrect node, try to fix it
    while (current != target) {
        vector<int> next = current;
        bool changed = false;
        
        for (int i = 0; i < n; i++) {
            if (current[i] != target[i]) {
                // Try to change node i to target color
                // Check if any neighbor has the target color
                for (int neighbor : adj[i]) {
                    if (current[neighbor] == target[i]) {
                        next[i] = target[i];
                        changed = true;
                        break;
                    }
                }
            }
        }
        
        if (!changed) {
            // If we can't directly fix any node, we need to propagate colors
            // Find a node that needs to be changed and try to get the right color to its neighbors
            for (int i = 0; i < n; i++) {
                if (current[i] != target[i]) {
                    // Look for a path to spread the target color
                    for (int neighbor : adj[i]) {
                        for (int neighbor2 : adj[neighbor]) {
                            if (current[neighbor2] == target[i]) {
                                next[neighbor] = target[i];
                                changed = true;
                                break;
                            }
                        }
                        if (changed) break;
                    }
                    if (changed) break;
                }
            }
        }
        
        if (!changed) {
            // Last resort: just propagate any different color
            for (int i = 0; i < n; i++) {
                for (int neighbor : adj[i]) {
                    if (current[neighbor] != current[i]) {
                        next[i] = current[neighbor];
                        changed = true;
                        break;
                    }
                }
                if (changed) break;
            }
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