#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <queue>
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

    vector<long long> s(n + 1);
    for (int i = 1; i <= n; ++i) {
        cout << "? 1 1 " << i << endl;
        long long response;
        cin >> response;
        s[i] = response;
    }

    vector<int> p0;
    for (int i = 1; i <= n; ++i) {
        if (s[i] % 2 != 0) { // d_r(i) is even if f(i) is odd
            p0.push_back(i);
        }
    }
    
    // For a negative odd number s, s%2 might be -1. s%2 != 0 is a robust check.
    // An alternative for d_r(i) even (i.e. s[i] odd): (s[i]%2 + 2)%2 == 1

    int r_cand = -1;
    for (int node : p0) {
        if (abs(s[node]) == 1) {
            r_cand = node;
            break;
        }
    }

    vector<int> parent(n + 1, 0);
    vector<bool> visited(n + 1, false);
    queue<int> q;

    q.push(r_cand);
    visited[r_cand] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    vector<int> ans(n + 1);
    ans[r_cand] = s[r_cand];
    for (int i = 1; i <= n; ++i) {
        if (i != r_cand) {
            ans[i] = s[i] - s[parent[i]];
        }
    }

    cout << "! ";
    for (int i = 1; i <= n; ++i) {
        cout << ans[i] << (i == n ? "" : " ");
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