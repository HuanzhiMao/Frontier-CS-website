#include <bits/stdc++.h>
using namespace std;

int main() {
    int t;
    cin >> t;
    for (int test = 0; test < t; test++) {
        int n;
        cin >> n;
        vector<vector<int>> adj(n + 1);
        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        vector<int> parent(n + 1), depth(n + 1), entry(n + 1), exit_(n + 1);
        vector<vector<int>> children(n + 1);
        int timer = 0;
        function<void(int, int, int)> dfs_build = [&](int u, int par, int dep) {
            parent[u] = par;
            depth[u] = dep;
            entry[u] = timer++;
            for (int v : adj[u]) {
                if (v != par) {
                    children[u].push_back(v);
                    dfs_build(v, u, dep + 1);
                }
            }
            exit_[u] = timer;
        };
        dfs_build(1, 0, 0);
        vector<long long> count(n + 1, 0);
        vector<int> active_c;
        for (int i = 1; i <= n; i++) {
            count[i] = 1;
            active_c.push_back(i);
        }
        long long total_possible = n;
        int queries = 0;
        while (true) {
            if (active_c.size() <= 1) {
                int ans = active_c.empty() ? 1 : active_c[0];
                cout << "! " << ans << endl;
                cout.flush();
                break;
            }
            vector<long long> sub_sum(n + 1, 0);
            function<void(int)> compute_subsum = [&](int u) {
                sub_sum[u] = count[u];
                for (int v : children[u]) {
                    compute_subsum(v);
                    sub_sum[u] += sub_sum[v];
                }
            };
            compute_subsum(1);
            long long best_mn = -1;
            int best_dep = INT_MAX;
            int best_x = -1;
            for (int xx = 1; xx <= n; xx++) {
                long long s1 = sub_sum[xx];
                long long s0 = total_possible - s1;
                long long mn = min(s0, s1);
                int d = depth[xx];
                if (mn > best_mn || (mn == best_mn && d < best_dep) || (mn == best_mn && d == best_dep && xx < best_x)) {
                    best_mn = mn;
                    best_dep = d;
                    best_x = xx;
                }
            }
            cout << "? " << best_x << endl;
            cout.flush();
            int r;
            cin >> r;
            queries++;
            vector<long long> new_count(n + 1, 0);
            set<int> temp_active;
            for (int c : active_c) {
                bool in = (entry[best_x] <= entry[c] && entry[c] < exit_[best_x]);
                int resp = in ? 1 : 0;
                int nc = (resp == 0 && c != 1) ? parent[c] : c;
                if (resp == r) {
                    new_count[nc] += count[c];
                    temp_active.insert(nc);
                }
            }
            count = new_count;
            active_c = vector<int>(temp_active.begin(), temp_active.end());
            total_possible = 0;
            for (int i = 1; i <= n; i++) total_possible += count[i];
        }
    }
    return 0;
}