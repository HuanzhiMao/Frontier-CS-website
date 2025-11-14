#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj;
vector<int> parent;
vector<int> depth;
vector<int> values;
int n, root;

void dfs(int u, int p, int d) {
    parent[u] = p;
    depth[u] = d;
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u, d + 1);
        }
    }
}

int query1(vector<int>& nodes) {
    cout << "? 1 " << nodes.size();
    for (int node : nodes) {
        cout << " " << node;
    }
    cout << endl;
    cout.flush();
    int result;
    cin >> result;
    return result;
}

void query2(int node) {
    cout << "? 2 " << node << endl;
    cout.flush();
}

void solve() {
    cin >> n;
    adj.assign(n + 1, vector<int>());
    parent.assign(n + 1, -1);
    depth.assign(n + 1, 0);
    values.assign(n + 1, 0);
    
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    // Find root by checking which node gives minimum sum when queried alone
    int min_sum = INT_MAX;
    root = 1;
    for (int i = 1; i <= n; i++) {
        vector<int> single = {i};
        int sum = query1(single);
        if (sum < min_sum) {
            min_sum = sum;
            root = i;
        }
    }
    
    // Build tree from root
    dfs(root, -1, 0);
    
    // Get root value
    vector<int> root_query = {root};
    values[root] = query1(root_query);
    
    // For each non-root node, find its value
    for (int u = 1; u <= n; u++) {
        if (u == root) continue;
        
        // Query path from root to u
        vector<int> path_query = {u};
        int path_sum = query1(path_query);
        
        // Query path from root to parent of u
        if (parent[u] != root) {
            vector<int> parent_query = {parent[u]};
            int parent_sum = query1(parent_query);
            values[u] = path_sum - parent_sum;
        } else {
            values[u] = path_sum - values[root];
        }
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << values[i];
    }
    cout << endl;
    cout.flush();
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    
    return 0;
}