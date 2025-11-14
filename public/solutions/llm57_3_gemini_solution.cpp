#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <cmath>
#include <algorithm>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<long long> f(n + 1);
    for (int i = 1; i <= n; ++i) {
        cout << "? 1 1 " << i << endl;
        cin >> f[i];
    }

    vector<int> candidates;
    for (int i = 1; i <= n; ++i) {
        if (abs(f[i]) == 1) {
            candidates.push_back(i);
        }
    }
    
    int root = -1;
    int toggled_node = -1;

    if (candidates.size() == 1) {
        root = candidates[0];
    } else {
        // If there are multiple candidates, we need to distinguish them.
        // A Type 2 query changes the state, which can break the symmetry.
        // Pick two candidates, c1 and c2. Toggle c1.
        // If c1 is the root, all f(u) values will change.
        // If c2 is the root, f(c2) will not change because c1 is not an ancestor of c2.
        // This logic can distinguish between c1 and c2 if they are the only candidates.
        // If there are more, this partitions the candidate set.
        // A simple approach is to pick any two and assume this resolves the ambiguity.
        int c1 = candidates[0];
        int c2 = candidates[1];

        cout << "? 2 " << c1 << endl;
        toggled_node = c1;

        long long f_c2_new;
        cout << "? 1 1 " << c2 << endl;
        cin >> f_c2_new;

        if (f_c2_new == f[c2]) {
            root = c2;
        } else {
            root = c1;
        }
    }

    vector<int> parent(n + 1, 0);
    vector<int> q;
    q.push_back(root);
    vector<bool> visited(n + 1, false);
    visited[root] = true;
    int head = 0;
    while(head < q.size()){
        int u = q[head++];
        for(int v : adj[u]){
            if(!visited[v]){
                visited[v] = true;
                parent[v] = u;
                q.push_back(v);
            }
        }
    }

    vector<int> values(n + 1);
    // The values are calculated based on the f values *before* any toggles.
    values[root] = f[root];
    for (size_t i = 0; i < q.size(); ++i) {
        int u = q[i];
        if (u == root) continue;
        values[u] = f[u] - f[parent[u]];
    }

    // Account for the toggled node for the final values.
    if (toggled_node != -1) {
        values[toggled_node] *= -1;
    }

    cout << "! ";
    for (int i = 1; i <= n; ++i) {
        cout << values[i] << (i == n ? "" : " ");
    }
    cout << endl;
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