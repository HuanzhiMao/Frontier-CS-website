#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj;
vector<int> depth;
vector<int> parent;

void dfs(int u, int p, int d) {
    depth[u] = d;
    parent[u] = p;
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u, d + 1);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        int n;
        cin >> n;
        
        adj.assign(n + 1, vector<int>());
        depth.assign(n + 1, 0);
        parent.assign(n + 1, -1);
        
        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        
        // Find root by checking which node gives minimum sum when treated as root
        int root = 1;
        int min_sum = INT_MAX;
        
        for (int candidate = 1; candidate <= n; candidate++) {
            dfs(candidate, -1, 0);
            
            cout << "? 1 " << n;
            for (int i = 1; i <= n; i++) {
                cout << " " << i;
            }
            cout << endl;
            cout.flush();
            
            int sum;
            cin >> sum;
            
            if (sum < min_sum) {
                min_sum = sum;
                root = candidate;
            }
        }
        
        // Now we know the root, compute tree with correct root
        dfs(root, -1, 0);
        
        vector<int> values(n + 1, 0);
        
        // Get path sums for all nodes
        vector<int> path_sums(n + 1);
        for (int i = 1; i <= n; i++) {
            cout << "? 1 1 " << i << endl;
            cout.flush();
            cin >> path_sums[i];
        }
        
        // Compute values bottom-up
        vector<pair<int, int>> nodes_by_depth;
        for (int i = 1; i <= n; i++) {
            nodes_by_depth.push_back({depth[i], i});
        }
        sort(nodes_by_depth.rbegin(), nodes_by_depth.rend());
        
        for (auto [d, u] : nodes_by_depth) {
            if (u == root) {
                values[u] = path_sums[u];
            } else {
                values[u] = path_sums[u] - path_sums[parent[u]];
            }
        }
        
        cout << "!";
        for (int i = 1; i <= n; i++) {
            cout << " " << values[i];
        }
        cout << endl;
        cout.flush();
    }
    
    return 0;
}