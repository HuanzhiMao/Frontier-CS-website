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
    
    vector<vector<int>> states;
    states.push_back(initial);
    vector<int> current = initial;
    
    while (current != target) {
        vector<int> next = current;
        
        for (int i = 0; i < n; i++) {
            if (current[i] != target[i]) {
                // Try to change node i to target color
                bool canChange = false;
                
                // Check if any neighbor has the target color
                for (int neighbor : adj[i]) {
                    if (current[neighbor] == target[i]) {
                        canChange = true;
                        break;
                    }
                }
                
                if (canChange) {
                    next[i] = target[i];
                }
            }
        }
        
        // If no progress made with greedy approach, try to propagate colors
        if (next == current) {
            for (int i = 0; i < n; i++) {
                if (current[i] != target[i]) {
                    // Try to get a color from neighbors that might help
                    for (int neighbor : adj[i]) {
                        if (current[neighbor] != current[i]) {
                            next[i] = current[neighbor];
                            break;
                        }
                    }
                    if (next[i] != current[i]) break;
                }
            }
        }
        
        states.push_back(next);
        current = next;
    }
    
    cout << states.size() - 1 << "\n";
    for (auto& state : states) {
        for (int i = 0; i < n; i++) {
            cout << state[i];
            if (i < n - 1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}