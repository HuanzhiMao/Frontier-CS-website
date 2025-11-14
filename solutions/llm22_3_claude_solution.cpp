#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    vector<int> parent(n + 1);
    parent[1] = 0;
    
    for (int i = 2; i <= n; i++) {
        int p;
        cin >> p;
        parent[i] = p;
        adj[p].push_back(i);
        adj[i].push_back(p);
    }
    
    // Find leaves for outer ring
    vector<int> leaves;
    for (int i = 1; i <= n; i++) {
        if (adj[i].size() == 1) {
            leaves.push_back(i);
        }
    }
    
    // Add outer ring edges
    int k = leaves.size();
    for (int i = 0; i < k; i++) {
        int u = leaves[i];
        int v = leaves[(i + 1) % k];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    // Create tree decomposition
    // Simple approach: create bags for each vertex and its neighbors
    vector<vector<int>> bags;
    vector<pair<int, int>> tree_edges;
    
    for (int i = 1; i <= n; i++) {
        vector<int> bag;
        bag.push_back(i);
        for (int j : adj[i]) {
            if (bag.size() < 4) {
                bag.push_back(j);
            }
        }
        bags.push_back(bag);
    }
    
    // Connect bags in a path
    for (int i = 1; i < bags.size(); i++) {
        tree_edges.push_back({i, i + 1});
    }
    
    // Output
    cout << bags.size() << "\n";
    for (auto& bag : bags) {
        cout << bag.size();
        for (int x : bag) {
            cout << " " << x;
        }
        cout << "\n";
    }
    
    for (auto& edge : tree_edges) {
        cout << edge.first << " " << edge.second << "\n";
    }
    
    return 0;
}