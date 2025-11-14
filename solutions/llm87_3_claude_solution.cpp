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
                // Try to change this node to target color
                for (int neighbor : adj[i]) {
                    if (current[neighbor] == target[i]) {
                        next[i] = target[i];
                        break;
                    }
                }
            }
        }
        
        // If no progress made with direct neighbors, try to make intermediate moves
        if (next == current) {
            for (int i = 0; i < n; i++) {
                if (current[i] != target[i]) {
                    // Find a path to a node with target color
                    queue<int> q;
                    vector<int> parent(n, -1);
                    vector<bool> visited(n, false);
                    
                    q.push(i);
                    visited[i] = true;
                    
                    int found = -1;
                    while (!q.empty() && found == -1) {
                        int u = q.front();
                        q.pop();
                        
                        for (int v : adj[u]) {
                            if (!visited[v]) {
                                visited[v] = true;
                                parent[v] = u;
                                q.push(v);
                                
                                if (current[v] == target[i]) {
                                    found = v;
                                    break;
                                }
                            }
                        }
                    }
                    
                    if (found != -1) {
                        // Propagate color backwards along the path
                        vector<int> path;
                        int cur = found;
                        while (cur != -1) {
                            path.push_back(cur);
                            cur = parent[cur];
                        }
                        
                        if (path.size() >= 2) {
                            int propagate_from = path[path.size() - 2];
                            for (int neighbor : adj[propagate_from]) {
                                if (current[neighbor] == target[i]) {
                                    next[propagate_from] = target[i];
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
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