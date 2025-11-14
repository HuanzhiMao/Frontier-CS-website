#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj;
vector<bool> visited;
vector<int> path;
vector<int> best_path;
int n, m;

void dfs(int u) {
    visited[u] = true;
    path.push_back(u);
    
    if (path.size() > best_path.size()) {
        best_path = path;
    }
    
    if (path.size() == n) {
        return;
    }
    
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs(v);
            if (best_path.size() == n) return;
        }
    }
    
    path.pop_back();
    visited[u] = false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> n >> m;
    
    vector<int> a(10);
    for (int i = 0; i < 10; i++) {
        cin >> a[i];
    }
    
    adj.resize(n + 1);
    visited.resize(n + 1);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
    }
    
    for (int start = 1; start <= n; start++) {
        fill(visited.begin(), visited.end(), false);
        path.clear();
        dfs(start);
        if (best_path.size() == n) break;
    }
    
    cout << best_path.size() << "\n";
    for (int i = 0; i < best_path.size(); i++) {
        if (i > 0) cout << " ";
        cout << best_path[i];
    }
    cout << "\n";
    
    return 0;
}