#include <bits/stdc++.h>
using namespace std;

struct Solver {
    int n;
    vector<vector<int>> g;
    vector<int> parent, depth, tin, tout, euler;
    int timer;
    vector<char> present, present2;
    vector<int> Pvec;
    vector<int> prefix;
    vector<int> markParent;
    int markStamp;

    void dfs(int u, int p) {
        parent[u] = (p == 0 ? 1 : p);
        depth[u] = (p == 0 ? 0 : depth[p] + 1);
        tin[u] = ++timer;
        euler[timer] = u;
        for (int v : g[u]) {
            if (v == p) continue;
            dfs(v, u);
        }
        tout[u] = timer;
    }

    inline bool inSub(int x, int y) {
        return tin[x] <= tin[y] && tin[y] <= tout[x];
    }

    void build_prefix() {
        prefix[0] = 0;
        for (int i = 1; i <= n; ++i) {
            int node = euler[i];
            prefix[i] = prefix[i-1] + (present[node] ? 1 : 0);
        }
    }

    int choose_best_x() {
        build_prefix();
        int bestX = 1;
        int bestWorst = INT_MAX;
        int bestDepth = INT_MAX;

        for (int x = 1; x <= n; ++x) {
            int a = prefix[tout[x]] - prefix[tin[x]-1];

            markStamp++;
            int b = 0;
            for (int y : Pvec) {
                if (!inSub(x, y)) {
                    int py = (y == 1 ? 1 : parent[y]);
                    if (markParent[py] != markStamp) {
                        markParent[py] = markStamp;
                        ++b;
                    }
                }
            }
            int worst = max(a, b);
            if (worst < bestWorst || (worst == bestWorst && depth[x] < bestDepth)) {
                bestWorst = worst;
                bestDepth = depth[x];
                bestX = x;
            }
        }
        return bestX;
    }

    void solve_case() {
        g.assign(n + 1, {});
        parent.assign(n + 1, 0);
        depth.assign(n + 1, 0);
        tin.assign(n + 1, 0);
        tout.assign(n + 1, 0);
        euler.assign(n + 1, 0);
        timer = 0;

        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }

        dfs(1, 0);

        present.assign(n + 1, 0);
        present2.assign(n + 1, 0);
        prefix.assign(n + 1, 0);
        markParent.assign(n + 1, 0);
        markStamp = 0;

        Pvec.clear();
        for (int i = 1; i <= n; ++i) {
            present[i] = 1;
            Pvec.push_back(i);
        }

        int ops = 0;
        while ((int)Pvec.size() > 1) {
            int x = choose_best_x();
            cout << "? " << x << endl;
            cout.flush();

            int ans;
            if (!(cin >> ans)) return;
            if (ans == -1) return;

            ++ops;
            if (ans == 1) {
                vector<int> newP;
                for (int y : Pvec) {
                    if (inSub(x, y)) {
                        newP.push_back(y);
                    } else {
                        present[y] = 0;
                    }
                }
                Pvec.swap(newP);
            } else {
                fill(present2.begin(), present2.end(), 0);
                vector<int> newP;
                for (int y : Pvec) {
                    if (!inSub(x, y)) {
                        int ny = (y == 1 ? 1 : parent[y]);
                        if (!present2[ny]) {
                            present2[ny] = 1;
                            newP.push_back(ny);
                        }
                    }
                }
                // Update present with present2
                fill(present.begin(), present.end(), 0);
                for (int v : newP) present[v] = 1;
                Pvec.swap(newP);
            }
            if (ops >= 160) break;
        }

        int answer = 1;
        if (!Pvec.empty()) answer = Pvec[0];
        cout << "! " << answer << endl;
        cout.flush();
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
        solver.n = n;
        solver.solve_case();
    }
    return 0;
}