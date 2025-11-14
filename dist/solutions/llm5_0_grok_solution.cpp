#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    cin >> n >> m;
    vector<int> a(10);
    for (int i = 0; i < 10; i++) {
        cin >> a[i];
    }
    vector<vector<int>> adj(n + 1);
    vector<vector<int>> rev(n + 1);
    vector<int> in_deg(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        rev[v].push_back(u);
        in_deg[v]++;
    }
    // Try Kahn's algorithm to check if DAG and get topo
    vector<int> temp_deg = in_deg;
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (temp_deg[i] == 0) {
            q.push(i);
        }
    }
    vector<int> topo;
    int cnt = 0;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        topo.push_back(v);
        cnt++;
        for (int u : adj[v]) {
            temp_deg[u]--;
            if (temp_deg[u] == 0) {
                q.push(u);
            }
        }
    }
    vector<int> best_path;
    if (cnt == n) {
        // DAG, compute longest path
        vector<int> dp(n + 1, 0);
        vector<int> predecessor(n + 1, -1);
        for (int i = (int)topo.size() - 1; i >= 0; i--) {
            int v = topo[i];
            dp[v] = 1;
            for (int u : adj[v]) {
                if (dp[u] + 1 > dp[v]) {
                    dp[v] = dp[u] + 1;
                    predecessor[v] = u;
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
        int cur = start;
        while (cur != -1) {
            path.push_back(cur);
            cur = predecessor[cur];
        }
        best_path = path;
    } else {
        // Has cycles, use greedy extension
        // Find start with max degree
        int max_deg = -1;
        int start = -1;
        for (int i = 1; i <= n; i++) {
            int d = (int)adj[i].size() + (int)rev[i].size();
            if (d > max_deg || (d == max_deg && i < start)) {
                max_deg = d;
                start = i;
            }
        }
        if (start == -1) start = 1; // fallback
        // Run for two modes
        for (int mode = 0; mode < 2; mode++) {
            vector<int> path;
            vector<char> visited(n + 1, 0);
            path.push_back(start);
            visited[start] = 1;
            bool extended = true;
            while (extended) {
                extended = false;
                // Try extend end
                int v = path.back();
                int chosen = -1;
                int best_score = (mode == 0 ? -1 : 1000000);
                for (int u : adj[v]) {
                    if (!visited[u]) {
                        int score = (int)adj[u].size();
                        bool better = (mode == 0 ? score > best_score : score < best_score);
                        if (better) {
                            best_score = score;
                            chosen = u;
                        }
                    }
                }
                if (chosen != -1) {
                    path.push_back(chosen);
                    visited[chosen] = 1;
                    extended = true;
                    continue;
                }
                // Try extend front
                v = path.front();
                chosen = -1;
                best_score = (mode == 0 ? -1 : 1000000);
                for (int u : rev[v]) {
                    if (!visited[u]) {
                        int score = (int)adj[u].size();
                        bool better = (mode == 0 ? score > best_score : score < best_score);
                        if (better) {
                            best_score = score;
                            chosen = u;
                        }
                    }
                }
                if (chosen != -1) {
                    path.insert(path.begin(), chosen);
                    visited[chosen] = 1;
                    extended = true;
                }
            }
            if (path.size() > best_path.size()) {
                best_path = path;
            }
        }
    }
    cout << best_path.size() << '\n';
    for (size_t i = 0; i < best_path.size(); i++) {
        if (i > 0) cout << ' ';
        cout << best_path[i];
    }
    cout << '\n';
    return 0;
}