#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj;
vector<int> parent;
vector<int> depth;
vector<int> values;
int root;
int n;

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
    for (int u : nodes) {
        cout << " " << u;
    }
    cout << endl;
    cout.flush();
    int result;
    cin >> result;
    return result;
}

void query2(int u) {
    cout << "? 2 " << u << endl;
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
    int minSum = INT_MAX;
    root = 1;
    for (int i = 1; i <= n; i++) {
        vector<int> nodes = {i};
        int sum = query1(nodes);
        if (sum < minSum) {
            minSum = sum;
            root = i;
        }
    }
    
    // Build tree from root
    dfs(root, -1, 0);
    
    // Get path sums for all nodes
    vector<int> pathSum(n + 1);
    for (int i = 1; i <= n; i++) {
        vector<int> nodes = {i};
        pathSum[i] = query1(nodes);
    }
    
    // Calculate node values
    values[root] = pathSum[root];
    
    vector<pair<int, int>> nodesByDepth;
    for (int i = 1; i <= n; i++) {
        nodesByDepth.push_back({depth[i], i});
    }
    sort(nodesByDepth.begin(), nodesByDepth.end());
    
    for (auto& p : nodesByDepth) {
        int u = p.second;
        if (u != root) {
            values[u] = pathSum[u] - pathSum[parent[u]];
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