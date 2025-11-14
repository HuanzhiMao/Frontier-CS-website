#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<pair<int, int>> edges;
    vector<set<int>> adj(n + 1);
    
    // For each pair of nodes, find if they are adjacent
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            // Find a third node different from i and j
            int k = -1;
            for (int t = 1; t <= n; t++) {
                if (t != i && t != j) {
                    k = t;
                    break;
                }
            }
            
            // Query with i, j, k
            cout << "0 " << i << " " << j << " " << k << endl;
            cout.flush();
            int result;
            cin >> result;
            
            // If the result is i or j, then i and j are adjacent
            if (result == i || result == j) {
                if (adj[i].find(j) == adj[i].end()) {
                    edges.push_back({i, j});
                    adj[i].insert(j);
                    adj[j].insert(i);
                }
            }
        }
    }
    
    cout << "1";
    for (auto& edge : edges) {
        cout << " " << edge.first << " " << edge.second;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}