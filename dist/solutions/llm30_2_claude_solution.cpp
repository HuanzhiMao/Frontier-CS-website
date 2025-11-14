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
    depth.assign(n + 1, 0);
    parent.assign(n + 1, -1);
    
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u, v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    dfs(1, -1, 0);
    
    int current = 1;
    
    while (true) {
        if (adj[current].empty() || (adj[current].size() == 1 && current != 1)) {
            // Leaf node
            cout << "! " << current << endl;
            cout.flush();
            return;
        }
        
        bool found = false;
        for (int child : adj[current]) {
            if (child != parent[current]) {
                int res = query(child);
                if (res == 1) {
                    current = child;
                    found = true;
                    break;
                }
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