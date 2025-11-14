#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    vector<pair<int, int>> edges;
    
    // For each vertex, find its neighbors
    for (int u = 1; u <= n; u++) {
        for (int v = u + 1; v <= n; v++) {
            // Check if u and v are adjacent by testing if any other vertex lies on their Steiner tree
            bool adjacent = true;
            
            for (int w = 1; w <= n && adjacent; w++) {
                if (w == u || w == v) continue;
                
                cout << "? 2 " << w << " " << u << " " << v << endl;
                cout.flush();
                
                int response;
                cin >> response;
                
                if (response == -1) return 0;
                
                if (response == 1) {
                    adjacent = false;
                }
            }
            
            if (adjacent) {
                adj[u].push_back(v);
                adj[v].push_back(u);
                edges.push_back({u, v});
            }
        }
    }
    
    cout << "!" << endl;
    for (auto& edge : edges) {
        cout << edge.first << " " << edge.second << endl;
    }
    cout.flush();
    
    return 0;
}