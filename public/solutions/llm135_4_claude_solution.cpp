#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int k, n;
    cin >> k >> n;
    
    vector<vector<int>> adj(n);
    
    // Find adjacent pairs by testing triplets
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int l = j + 1; l < n; l++) {
                cout << "? " << i << " " << j << " " << l << endl;
                cout.flush();
                
                int r;
                cin >> r;
                
                set<pair<int, int>> closest;
                for (int p = 0; p < r; p++) {
                    int a, b;
                    cin >> a >> b;
                    closest.insert({min(a, b), max(a, b)});
                }
                
                // If only one pair is closest, it's likely adjacent
                if (r == 1) {
                    auto p = *closest.begin();
                    adj[p.first].push_back(p.second);
                    adj[p.second].push_back(p.first);
                }
            }
        }
    }
    
    // Clean adjacency list - each node should have exactly 2 neighbors
    for (int i = 0; i < n; i++) {
        sort(adj[i].begin(), adj[i].end());
        adj[i].erase(unique(adj[i].begin(), adj[i].end()), adj[i].end());
    }
    
    // Find the cycle
    vector<int> result;
    vector<bool> visited(n, false);
    
    // Start from node 0
    int current = 0;
    int prev = -1;
    
    while (result.size() < n) {
        result.push_back(current);
        visited[current] = true;
        
        int next = -1;
        for (int neighbor : adj[current]) {
            if (neighbor != prev) {
                next = neighbor;
                break;
            }
        }
        
        if (next == -1) break;
        prev = current;
        current = next;
    }
    
    // Output result
    cout << "!";
    for (int x : result) {
        cout << " " << x;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}