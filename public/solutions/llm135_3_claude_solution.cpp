#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int k, n;
    cin >> k >> n;
    
    vector<vector<int>> adj(n);
    
    // Find adjacencies using queries
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (adj[i].size() == 2 && adj[j].size() == 2) continue;
            
            // Find a third vertex to query with
            int third = -1;
            for (int t = 0; t < n; t++) {
                if (t != i && t != j) {
                    third = t;
                    break;
                }
            }
            
            cout << "? " << i << " " << j << " " << third << endl;
            cout.flush();
            
            int r;
            cin >> r;
            
            bool adjacent = false;
            for (int p = 0; p < r; p++) {
                int a, b;
                cin >> a >> b;
                if ((a == i && b == j) || (a == j && b == i)) {
                    adjacent = true;
                }
            }
            
            if (adjacent) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }
    
    // Find the cycle
    vector<int> result;
    vector<bool> visited(n, false);
    
    // Start from vertex 0
    int current = 0;
    visited[current] = true;
    result.push_back(current);
    
    int prev = -1;
    while (result.size() < n) {
        for (int next : adj[current]) {
            if (next != prev) {
                result.push_back(next);
                visited[next] = true;
                prev = current;
                current = next;
                break;
            }
        }
    }
    
    cout << "!";
    for (int x : result) {
        cout << " " << x;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}