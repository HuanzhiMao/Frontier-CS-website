#include <bits/stdc++.h>
using namespace std;

struct Solver {
    int n, LOG;
    vector<vector<int>> g, up;
    vector<int> depth, tin, tout, euler;
    int timer;

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        depth.assign(n + 1, 0);
        tin.assign(n + 1, 0);
        tout.assign(n + 1, 0);
        euler.assign(n, 0);
        timer = 0;
        LOG = 1;
        while ((1 << LOG) <= n) LOG++;
        up.assign(LOG, vector<int>(n + 1, 1));
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int p) {
        up[0][u] = p;
        depth[u] = (u == p ? 0 : depth[p] + 1);
        tin[u] = timer;
        euler[timer] = u;
        timer++;
        for (int v : g[u]) {
            if (v == p) continue;
            dfs(v, u);
        }
        tout[u] = timer;
    }

    void build() {
        dfs(1, 1);
        for (int k = 1; k < LOG; ++k) {
            for (int v = 1; v <= n; ++v) {
                up[k][v] = up[k - 1][up[k - 1][v]];
            }
        }
    }

    int climb(int u, int k) {
        if (k <= 0) return u;
        if (k > depth[u]) return 1;
        for (int i = 0; i < LOG; ++i) {
            if (k & (1 << i)) u = up[i][u];
        }
        return u;
    }

    inline bool in_subtree(int x, int y) {
        return tin[x] <= tin[y] && tout[y] <= tout[x];
    }

    void solve_case() {
        vector<int> candidates(n);
        iota(candidates.begin(), candidates.end(), 1);
        int z = 0; // number of zero answers so far

        int query_cnt = 0;

        while (true) {
            // Build weights over current positions
            vector<int> arr(n, 0);
            for (int u : candidates) {
                int y = climb(u, z);
                arr[tin[y]]++;
            }
            int total = (int)candidates.size();

            // Check if only one possible current position remains
            int unique_idx = -1, cnt_unique = 0;
            for (int i = 0; i < n; ++i) {
                if (arr[i] > 0) {
                    cnt_unique++;
                    unique_idx = i;
                    if (cnt_unique > 1) break;
                }
            }
            if (cnt_unique == 1) {
                int y = euler[unique_idx];
                cout << "! " << y << endl;
                cout.flush();
                return;
            }

            // Compute prefix sums over Euler to get subtree counts quickly
            vector<int> pref(n + 1, 0);
            for (int i = 0; i < n; ++i) pref[i + 1] = pref[i] + arr[i];

            auto subtree_count = [&](int x) -> int {
                return pref[tout[x]] - pref[tin[x]];
            };

            int bestX = -1;
            int bestDiff = INT_MAX;
            for (int x = 1; x <= n; ++x) {
                int c1 = subtree_count(x);
                if (c1 == 0 || c1 == total) continue; // useless split
                int diff = abs(total - 2 * c1);
                if (diff < bestDiff) {
                    bestDiff = diff;
                    bestX = x;
                }
            }

            if (bestX == -1) {
                // Fallback: choose any node that has some but not all, should rarely happen.
                // Try any current position's node to split.
                // Since cnt_unique > 1, try pick a node from euler with arr>0 and then search deeper descendants.
                for (int i = 0; i < n && bestX == -1; ++i) {
                    if (arr[i] > 0) {
                        int candidate_node = euler[i];
                        // Try all nodes; pick first that gives partial split
                        for (int x = 1; x <= n; ++x) {
                            int c1 = subtree_count(x);
                            if (c1 > 0 && c1 < total) {
                                bestX = x;
                                break;
                            }
                        }
                    }
                }
                if (bestX == -1) {
                    // As a last resort (should not happen), answer root
                    cout << "! " << 1 << endl;
                    cout.flush();
                    return;
                }
            }

            cout << "? " << bestX << endl;
            cout.flush();
            int r;
            if (!(cin >> r)) exit(0);
            query_cnt++;
            if (query_cnt > 160) exit(0);

            vector<int> newCands;
            newCands.reserve(candidates.size());
            if (r == 1) {
                for (int u : candidates) {
                    int y = climb(u, z);
                    if (in_subtree(bestX, y)) newCands.push_back(u);
                }
            } else {
                for (int u : candidates) {
                    int y = climb(u, z);
                    if (!in_subtree(bestX, y)) newCands.push_back(u);
                }
                z++;
            }
            candidates.swap(newCands);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        cin >> n;
        Solver solver;
        solver.init(n);
        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            solver.add_edge(u, v);
        }
        solver.build();
        solver.solve_case();
    }
    return 0;
}