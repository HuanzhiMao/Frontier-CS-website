#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    
    // For each pair of vertices, check if they are adjacent
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            // Check if there's a direct edge between i and j
            // We do this by finding a vertex k such that k is on Steiner({i,j})
            // but when we add k to the query, some other vertex is no longer on the Steiner tree
            
            bool direct_edge = true;
            
            // Try to find a vertex k that lies on the path between i and j
            for (int k = 1; k <= n && direct_edge; k++) {
                if (k == i || k == j) continue;
                
                cout << "? 2 " << k << " " << i << " " << j << endl;
                cout.flush();
                
                int result;
                cin >> result;
                if (result == -1) return 0;
                
                if (result == 1) {
                    // k is on the path between i and j, so i and j are not directly connected
                    direct_edge = false;
                }
            }
            
            if (direct_edge) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }
    
    cout << "!" << endl;
    for (int i = 1; i <= n; i++) {
        for (int j : adj[i]) {
            if (i < j) {
                cout << i << " " << j << endl;
            }
        }
    }
    cout.flush();
    
    return 0;
}