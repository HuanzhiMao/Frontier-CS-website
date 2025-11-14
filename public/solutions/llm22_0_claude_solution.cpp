#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    
    for (int i = 2; i <= n; i++) {
        int p;
        cin >> p;
        adj[p].push_back(i);
        adj[i].push_back(p);
    }
    
    // Find leaf nodes
    vector<int> leaves;
    for (int i = 1; i <= n; i++) {
        if (adj[i].size() == 1) {
            leaves.push_back(i);
        }
    }
    
    // Create one node containing all vertices
    cout << 1 << "\n";
    cout << n;
    for (int i = 1; i <= n; i++) {
        cout << " " << i;
    }
    cout << "\n";
    
    return 0;
}