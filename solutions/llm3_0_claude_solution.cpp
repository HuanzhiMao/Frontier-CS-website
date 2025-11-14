#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int subtask, n;
    cin >> subtask >> n;
    
    vector<vector<int>> adj(n + 1);
    
    // Test all pairs to find adjacent ones
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            cout << "2 " << i << " " << j << endl;
            cout.flush();
            
            vector<int> response(2);
            cin >> response[0] >> response[1];
            
            if (response[1] == 1) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
            
            // Turn off the lights
            cout << "2 " << i << " " << j << endl;
            cout.flush();
            cin >> response[0] >> response[1];
        }
    }
    
    // Build the ring from adjacency information
    vector<int> result;
    vector<bool> visited(n + 1, false);
    
    // Start from node 1
    int current = 1;
    result.push_back(current);
    visited[current] = true;
    
    // Find the first neighbor to start the path
    int next = adj[current][0];
    
    while (result.size() < n) {
        result.push_back(next);
        visited[next] = true;
        
        // Find the next unvisited neighbor
        int prev = current;
        current = next;
        next = -1;
        
        for (int neighbor : adj[current]) {
            if (!visited[neighbor]) {
                next = neighbor;
                break;
            }
        }
        
        if (next == -1) break;
    }
    
    cout << "-1";
    for (int x : result) {
        cout << " " << x;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}