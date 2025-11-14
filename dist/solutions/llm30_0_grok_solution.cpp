#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    for (int test = 0; test < t; ++test) {
        int n;
        cin >> n;
        vector<vector<int>> adj(n + 1);
        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        vector<vector<int>> children(n + 1);
        vector<int> parent(n + 1), depth(n + 1);
        vector<bool> vis(n + 1, false);
        queue<int> q;
        q.push(1);
        vis[1] = true;
        parent[1] = 1;
        depth[1] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (!vis[v]) {
                    vis[v] = true;
                    parent[v] = u;
                    depth[v] = depth[u] + 1;
                    children[u].push_back(v);
                    q.push(v);
                }
            }
        }
        const int LOG = 13;
        vector<vector<int>> up(n + 1, vector<int>(LOG));
        for (int i = 1; i <= n; ++i) {
            up[i][0] = parent[i];
        }
        up[1][0] = 1;
        for (int j = 1; j < LOG; ++j) {
            for (int i = 1; i <= n; ++i) {
                up[i][j] = up[up[i][j - 1]][j - 1];
            }
        }
        vector<int> enter(n + 1), exitt(n + 1);
        int timer = 0;
        auto dfs_time = [&](auto self, int u, int par) -> void {
            enter[u] = timer++;
            for (int v : children[u]) {
                if (v != par) self(self, v, u);
            }
            exitt[u] = timer;
        };
        dfs_time(dfs_time, 1, -1);
        auto is_in_subtree = [&](int a, int b) -> bool {
            return enter[b] <= enter[a] && enter[a] < exitt[b];
        };
        auto get_ancestor = [&](int node, int dist) -> int {
            if (dist == 0) return node;
            int cur = node;
            for (int j = LOG - 1; j >= 0; --j) {
                if ((1 << j) <= dist) {
                    cur = up[cur][j];
                    dist -= (1 << j);
                }
            }
            return cur;
        };
        vector<int> curr_possible;
        curr_possible.reserve(n);
        for (int i = 1; i <= n; ++i) curr_possible.push_back(i);
        int k = 0;
        vector<int> count_cur(n + 1);
        vector<int> subtree_count(n + 1);
        auto dfs_sub = [&](auto self, int u, int par) -> int {
            subtree_count[u] = count_cur[u];
            for (int v : children[u]) {
                if (v != par) subtree_count[u] += self(self, v, u);
            }
            return subtree_count[u];
        };
        int queries = 0;
        while (true) {
            ++queries;
            if (queries > 160) {
                // Should not happen
                break;
            }
            fill(count_cur.begin(), count_cur.end(), 0);
            for (int s : curr_possible) {
                int m = min(k, depth[s]);
                int cur = get_ancestor(s, m);
                count_cur[cur]++;
            }
            int num_distinct = 0;
            int the_p = -1;
            bool multiple = false;
            for (int p = 1; p <= n && !multiple; ++p) {
                if (count_cur[p] > 0) {
                    ++num_distinct;
                    the_p = p;
                    if (num_distinct > 1) multiple = true;
                }
            }
            if (num_distinct == 1) {
                cout << "! " << the_p << endl;
                cout.flush();
                break;
            }
            dfs_sub(dfs_sub, 1, -1);
            int total = subtree_count[1];
            int best_diff = INT_MAX;
            int best_dep = INT_MAX;
            int best_x = -1;
            for (int u = 1; u <= n; ++u) {
                int yes = subtree_count[u];
                if (yes > 0 && yes < total) {
                    int dff = abs(yes - total / 2);
                    int dep = depth[u];
                    if (dff < best_diff || (dff == best_diff && dep < best_dep) || (dff == best_diff && dep == best_dep && u < best_x)) {
                        best_diff = dff;
                        best_dep = dep;
                        best_x = u;
                    }
                }
            }
            assert(best_x != -1);
            cout << "? " << best_x << endl;
            cout.flush();
            int res;
            cin >> res;
            vector<int> new_poss;
            new_poss.reserve(curr_possible.size());
            for (int s : curr_possible) {
                int m = min(k, depth[s]);
                int cur = get_ancestor(s, m);
                bool in = is_in_subtree(cur, best_x);
                if ((res == 1 && in) || (res == 0 && !in)) {
                    new_poss.push_back(s);
                }
            }
            curr_possible = move(new_poss);
            if (res == 0) ++k;
        }
    }
    return 0;
}