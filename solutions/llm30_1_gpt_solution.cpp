#include <bits/stdc++.h>
using namespace std;

struct Solver {
    int n;
    vector<vector<int>> g;
    vector<int> tin, tout, parent, depth;
    int timer;

    void dfs(int u, int p) {
        parent[u] = p;
        tin[u] = ++timer;
        for (int v : g[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
        tout[u] = timer;
    }

    inline bool in_subtree(int x, int y) {
        return tin[x] <= tin[y] && tout[y] <= tout[x];
    }

    int ask(int x) {
        cout << "? " << x << endl;
        cout.flush();
        int r;
        if (!(cin >> r)) exit(0);
        return r;
    }

    void answer(int x) {
        cout << "! " << x << endl;
        cout.flush();
    }

    void run() {
        cin >> n;
        g.assign(n + 1, {});
        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        tin.assign(n + 1, 0);
        tout.assign(n + 1, 0);
        parent.assign(n + 1, 0);
        depth.assign(n + 1, 0);
        timer = 0;
        dfs(1, 0);

        vector<int> S;
        S.reserve(n);
        for (int i = 1; i <= n; ++i) S.push_back(i);

        vector<int> cur(n + 1);
        for (int i = 1; i <= n; ++i) cur[i] = i;

        while (true) {
            if (S.empty()) {
                // Should not happen with a correct interactor; fallback to root.
                answer(1);
                return;
            }
            int example = cur[S[0]];
            bool allSame = true;
            for (int v : S) {
                if (cur[v] != example) { allSame = false; break; }
            }
            if (allSame) {
                answer(example);
                return;
            }

            vector<int> cntTime(n + 2, 0);
            for (int v : S) cntTime[tin[cur[v]]]++;
            vector<int> pref(n + 2, 0);
            for (int i = 1; i <= n; ++i) pref[i] = pref[i - 1] + cntTime[i];

            int sizeS = (int)S.size();
            int bestX = -1;
            int bestMaxRem = sizeS;
            for (int x = 2; x <= n; ++x) {
                int c = pref[tout[x]] - pref[tin[x] - 1];
                if (c == 0 || c == sizeS) continue;
                int maxRem = max(c, sizeS - c);
                if (maxRem < bestMaxRem) {
                    bestMaxRem = maxRem;
                    bestX = x;
                }
            }
            if (bestX == -1) {
                // Fallback: find any split if possible
                for (int x = 1; x <= n; ++x) {
                    int c = pref[tout[x]] - pref[tin[x] - 1];
                    if (0 < c && c < sizeS) {
                        bestX = x;
                        break;
                    }
                }
                if (bestX == -1) {
                    // No split means all cur positions coincide (should have been caught)
                    answer(example);
                    return;
                }
            }

            int y = ask(bestX);

            vector<int> newS;
            newS.reserve(S.size());
            for (int v : S) {
                int ans_v = in_subtree(bestX, cur[v]) ? 1 : 0;
                if (ans_v == y) {
                    if (y == 0 && cur[v] != 1) cur[v] = parent[cur[v]];
                    newS.push_back(v);
                }
            }
            S.swap(newS);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        Solver s;
        s.run();
    }
    return 0;
}