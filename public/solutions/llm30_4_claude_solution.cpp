#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj;
vector<int> depth;

void dfs(int u, int p, int d) {
    depth[u] = d;
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u, d + 1);
        }
    }
}

int query(int x) {
    cout << "? " << x << endl;
    cout.flush();
    int result;
    cin >> result;
    return result;
}

void answer(int x) {
    cout << "! " << x << endl;
    cout.flush();
}

int solve(int u, int p) {
    if (query(u) == 0) {
        return solve(p, -1);
    }
    
    vector<int> children;
    for (int v : adj[u]) {
        if (v != p) {
            children.push_back(v);
        }
    }
    
    if (children.empty()) {
        return u;
    }
    
    for (int child : children) {
        if (query(child) == 1) {
            return solve(child, u);
        }
    }
    
    return u;
}

int main() {
    int t;
    cin >> t;
    
    while (t--) {
        int n;
        cin >> n;
        
        adj.assign(n + 1, vector<int>());
        depth.assign(n + 1, 0);
        
        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        
        dfs(1, -1, 0);
        
        int result = solve(1, -1);
        answer(result);
    }
    
    return 0;
}