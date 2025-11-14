#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    vector<long long> t;
    int n;
    Fenwick(int _n) : n(_n), t(_n + 2, 0) {}
    void update(int idx, long long val) {
        while (idx <= n) {
            t[idx] += val;
            idx += idx & -idx;
        }
    }
    long long query(int idx) {
        long long sum = 0;
        while (idx > 0) {
            sum += t[idx];
            idx -= idx & -idx;
        }
        return sum;
    }
};

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
        vector<int> parent(n + 1, 0);
        vector<int> depth(n + 1, 0);
        vector<int> entry(n + 1, 0);
        vector<int> exitt(n + 1, 0);
        int timer = 0;
        function<void(int, int)> dfs = [&](int u, int par) {
            parent[u] = par;
            entry[u] = timer++;
            for (int v : adj[u]) {
                if (v != par) {
                    depth[v] = depth[u] + 1;
                    dfs(v, u);
                }
            }
            exitt[u] = timer;
        };
        dfs(1, -1);
        vector<int> curr(n + 1);
        for (int i = 1; i <= n; ++i) curr[i] = i;
        vector<int> possible(n);
        for (int i = 0; i < n; ++i) possible[i] = i + 1;
        int num_pos = n;
        Fenwick ft(n);
        ft.t.assign(ft.n + 2, 0);
        for (int idx : possible) {
            ft.update(entry[curr[idx]] + 1, 1);
        }
        int queries = 0;
        while (true) {
            ++queries;
            if (queries > 160) {
                // Should not happen, but to prevent infinite loop
                break;
            }
            if (num_pos == 0) {
                break;
            }
            int the_cur = curr[possible[0]];
            bool same = true;
            for (int j = 1; j < num_pos; ++j) {
                if (curr[possible[j]] != the_cur) {
                    same = false;
                    break;
                }
            }
            if (same) {
                cout << "! " << the_cur << endl;
                cout.flush();
                break;
            }
            int best_d = INT_MAX;
            int best_bal = -1;
            int best_xi = -1;
            for (int xi = 1; xi <= n; ++xi) {
                long long cyes = ft.query(exitt[xi]) - ft.query(entry[xi]);
                long long cno = num_pos - cyes;
                if (cyes > 0 && cno > 0) {
                    int mins = min(cyes, cno);
                    int d = depth[xi];
                    if (d < best_d || (d == best_d && mins > best_bal)) {
                        best_d = d;
                        best_bal = mins;
                        best_xi = xi;
                    }
                }
            }
            cout << "? " << best_xi << endl;
            cout.flush();
            int resp;
            cin >> resp;
            vector<int> new_poss;
            new_poss.reserve(num_pos);
            for (int j = 0; j < num_pos; ++j) {
                int idx = possible[j];
                int c = curr[idx];
                bool yes = (entry[best_xi] <= entry[c] && entry[c] < exitt[best_xi]);
                if ((yes ? 1 : 0) == resp) {
                    if (resp == 0 && c != 1) {
                        curr[idx] = parent[c];
                    }
                    new_poss.push_back(idx);
                }
            }
            possible = std::move(new_poss);
            num_pos = possible.size();
            ft.t.assign(ft.n + 2, 0);
            for (int j = 0; j < num_pos; ++j) {
                int idx = possible[j];
                ft.update(entry[curr[idx]] + 1, 1);
            }
        }
    }
    return 0;
}