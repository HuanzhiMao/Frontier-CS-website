#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    vector<int> a(10);
    for (int &x : a) cin >> x;
    vector<vector<int>> g(n + 1);
    vector<int> in_deg(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        in_deg[v]++;
    }
    vector<int> sources;
    for (int i = 1; i <= n; i++) {
        if (in_deg[i] == 0) sources.push_back(i);
    }
    vector<int> all_starts = sources;
    for (int i = 1; i <= n; i++) {
        if (in_deg[i] != 0) all_starts.push_back(i);
    }
    vector<int> order;
    vector<bool> visited(n + 1, false);
    for (int ii = 0; ii < all_starts.size(); ii++) {
        int i = all_starts[ii];
        if (visited[i]) continue;
        stack<pair<int, int>> stk;
        stk.push({i, 0});
        visited[i] = true;
        while (!stk.empty()) {
            auto &p = stk.top();
            int u = p.first;
            int &child = p.second;
            if (child < g[u].size()) {
                int v = g[u][child];
                child++;
                if (!visited[v]) {
                    visited[v] = true;
                    stk.push({v, 0});
                }
            } else {
                order.push_back(u);
                stk.pop();
            }
        }
    }
    reverse(order.begin(), order.end());
    vector<int> pos(n + 1, -1);
    for (int i = 0; i < n; i++) {
        pos[order[i]] = i;
    }
    vector<int> dp(n + 1, 0);
    for (int j = n - 1; j >= 0; j--) {
        int u = order[j];
        dp[u] = 1;
        for (int v : g[u]) {
            if (pos[v] > pos[u]) {
                dp[u] = max(dp[u], 1 + dp[v]);
            }
        }
    }
    int max_len = 0;
    int start = -1;
    for (int i = 1; i <= n; i++) {
        if (dp[i] > max_len) {
            max_len = dp[i];
            start = i;
        }
    }
    vector<int> path;
    if (start == -1) {
        path.push_back(1);
    } else {
        int cur = start;
        path.push_back(cur);
        while (true) {
            int next = -1;
            for (int v : g[cur]) {
                if (pos[v] > pos[cur] && dp[v] == dp[cur] - 1) {
                    next = v;
                    break;
                }
            }
            if (next == -1) break;
            path.push_back(next);
            cur = next;
        }
    }
    cout << path.size() << '\n';
    for (int i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) cout << ' ';
        else cout << '\n';
    }
    return 0;
}