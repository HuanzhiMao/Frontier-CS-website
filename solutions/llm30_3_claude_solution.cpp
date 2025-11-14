#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj;
vector<int> parent;
vector<int> depth;

void dfs(int u, int p, int d) {
    parent[u] = p;
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
    int res;
    cin >> res;
    return res;
}

void solve() {
    int n;
    cin >> n;
    
    adj.assign(n + 1, vector<int>());
    parent.assign(n + 1, -1);
    depth.assign(n + 1, 0);
    
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    dfs(1, -1, 0);
    
    int current = 1;
    
    while (true) {
        vector<int> children;
        for (int child : adj[current]) {
            if (child != parent[current]) {
                children.push_back(child);
            }
        }
        
        if (children.empty()) {
            cout << "! " << current << endl;
            cout.flush();
            return;
        }
        
        bool found = false;
        for (int child : children) {
            int res = query(child);
            if (res == 1) {
                current = child;
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "! " << current << endl;
            cout.flush();
            return;
        }
    }
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}