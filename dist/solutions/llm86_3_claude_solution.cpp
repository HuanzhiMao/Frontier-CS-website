#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    vector<pair<int, int>> edges;
    
    // For each potential edge, check if it exists
    for (int u = 1; u <= n; u++) {
        for (int v = u + 1; v <= n; v++) {
            // Find a third node different from u and v
            int w = -1;
            for (int i = 1; i <= n; i++) {
                if (i != u && i != v) {
                    w = i;
                    break;
                }
            }
            
            // Query with u, v, w
            cout << "0 " << u << " " << v << " " << w << endl;
            cout.flush();
            
            int response;
            cin >> response;
            
            // If response is u or v, then u and v are likely connected
            if (response == u || response == v) {
                adj[u].push_back(v);
                adj[v].push_back(u);
                edges.push_back({u, v});
                
                if (edges.size() == n - 1) {
                    goto output;
                }
            }
        }
    }
    
    output:
    cout << "1";
    for (auto edge : edges) {
        cout << " " << edge.first << " " << edge.second;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}