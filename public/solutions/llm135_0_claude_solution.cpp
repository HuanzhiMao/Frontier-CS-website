#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int k, n;
    cin >> k >> n;
    
    vector<vector<int>> adj(n);
    
    // Query all triples to build adjacency information
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int l = j + 1; l < n; l++) {
                cout << "? " << i << " " << j << " " << l << "\n";
                cout.flush();
                
                int r;
                cin >> r;
                
                set<pair<int, int>> closest;
                for (int p = 0; p < r; p++) {
                    int a, b;
                    cin >> a >> b;
                    closest.insert({min(a, b), max(a, b)});
                }
                
                // If only one pair is closest, those are adjacent
                if (r == 1) {
                    auto p = *closest.begin();
                    adj[p.first].push_back(p.second);
                    adj[p.second].push_back(p.first);
                }
            }
        }
    }
    
    // Find the cycle
    vector<int> path;
    vector<bool> visited(n, false);
    
    // Start from node 0
    int current = 0;
    path.push_back(current);
    visited[current] = true;
    
    while (path.size() < n) {
        bool found = false;
        for (int next : adj[current]) {
            if (!visited[next]) {
                path.push_back(next);
                visited[next] = true;
                current = next;
                found = true;
                break;
            }
        }
        
        if (!found) {
            // Backtrack or try different approach
            // This shouldn't happen in a proper cycle
            break;
        }
    }
    
    // If we don't have a complete path, try building it differently
    if (path.size() != n) {
        path.clear();
        fill(visited.begin(), visited.end(), false);
        
        // Find a node with exactly 2 neighbors (should be all in a cycle)
        int start = 0;
        for (int i = 0; i < n; i++) {
            if (adj[i].size() >= 2) {
                start = i;
                break;
            }
        }
        
        path.push_back(start);
        visited[start] = true;
        current = start;
        
        // Follow the path
        int prev = -1;
        while (path.size() < n) {
            int next = -1;
            for (int neighbor : adj[current]) {
                if (neighbor != prev && (!visited[neighbor] || neighbor == start)) {
                    next = neighbor;
                    break;
                }
            }
            
            if (next == -1 || (next == start && path.size() < n)) break;
            
            if (next != start) {
                path.push_back(next);
                visited[next] = true;
            }
            
            prev = current;
            current = next;
            
            if (current == start && path.size() == n) break;
        }
    }
    
    // Output the result
    cout << "!";
    for (int x : path) {
        cout << " " << x;
    }
    cout << "\n";
    cout.flush();
    
    return 0;
}