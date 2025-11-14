#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj;
vector<int> values;
int n;

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
    values.assign(n + 1, 0);
    
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    // Get f(1)
    vector<int> single = {1};
    int f1 = query1(single);
    
    // Get f(1) + f(2)
    vector<int> pair12 = {1, 2};
    int f12 = query1(pair12);
    int f2 = f12 - f1;
    
    // Get all other f values
    vector<int> f(n + 1);
    f[1] = f1;
    f[2] = f2;
    
    for (int i = 3; i <= n; i++) {
        vector<int> pair1i = {1, i};
        int f1i = query1(pair1i);
        f[i] = f1i - f1;
    }
    
    // Now we need to find the root and compute node values
    // Try each node as potential root
    for (int root = 1; root <= n; root++) {
        vector<int> nodeValues(n + 1);
        bool valid = true;
        
        // BFS from potential root
        vector<bool> visited(n + 1, false);
        queue<int> q;
        q.push(root);
        visited[root] = true;
        
        // Root value equals f(root)
        nodeValues[root] = f[root];
        if (abs(nodeValues[root]) != 1) {
            continue;
        }
        
        while (!q.empty() && valid) {
            int u = q.front();
            q.pop();
            
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                    
                    // f[v] = f[u] + value[v]
                    nodeValues[v] = f[v] - f[u];
                    if (abs(nodeValues[v]) != 1) {
                        valid = false;
                        break;
                    }
                }
            }
        }
        
        if (valid) {
            // Found the correct root and values
            values = nodeValues;
            break;
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
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}