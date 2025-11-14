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
                // Try to change this node's color
                for (int neighbor : adj[i]) {
                    if (current[neighbor] == target[i]) {
                        next[i] = target[i];
                        break;
                    }
                }
            }
        }
        
        // If no progress was made with direct neighbor copying, try spreading colors
        if (next == current) {
            for (int i = 0; i < n; i++) {
                if (current[i] == target[i]) {
                    // This node has the correct color, spread it to neighbors that need it
                    for (int neighbor : adj[i]) {
                        if (current[neighbor] != target[neighbor] && target[neighbor] == current[i]) {
                            next[neighbor] = current[i];
                            break;
                        }
                    }
                }
            }
        }
        
        current = next;
        steps.push_back(current);
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