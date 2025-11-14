#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> children;
vector<int> depth;

void dfs(int node, int parent, int d) {
    depth[node] = d;
    for (int child : children[node]) {
        if (child != parent) {
            dfs(child, node, d + 1);
        }
    }
}

int query(int x) {
    cout << "? " << x << endl;
    cout.flush();
    int response;
    cin >> response;
    return response;
}

void answer(int x) {
    cout << "! " << x << endl;
    cout.flush();
}

int solve(int node) {
    if (children[node].empty()) {
        return node;
    }
    
    // Sort children by depth (deepest first)
    vector<int> sorted_children = children[node];
    sort(sorted_children.begin(), sorted_children.end(), [](int a, int b) {
        return depth[a] > depth[b];
    });
    
    for (int child : sorted_children) {
        if (query(child) == 1) {
            return solve(child);
        }
    }
    
    return node;
}

int main() {
    int t;
    cin >> t;
    
    while (t--) {
        int n;
        cin >> n;
        
        children.assign(n + 1, vector<int>());
        depth.assign(n + 1, 0);
        
        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            if (u > v) swap(u, v);
            children[u].push_back(v);
        }
        
        dfs(1, -1, 0);
        
        int result = solve(1);
        answer(result);
    }
    
    return 0;
}