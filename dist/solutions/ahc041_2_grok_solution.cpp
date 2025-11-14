#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M, H;
    cin >> N >> M >> H;
    vector<int> A(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }
    vector<vector<int>> adj(N);
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 0; i < N; i++) {
        int x, y;
        cin >> x >> y;
    }
    vector<int> p(N, -1);
    vector<vector<int>> children(N);
    vector<long long> comp_sum(N);
    vector<int> comp_mh(N, 0);
    for (int i = 0; i < N; i++) {
        comp_sum[i] = A[i];
    }
    set<int> active_roots;
    for (int i = 0; i < N; i++) {
        active_roots.insert(i);
    }
    auto find_root = [&](auto self, int u) -> int {
        if (p[u] == -1) return u;
        return self(self, p[u]);
    };
    auto compute_depth = [&](auto self, int u) -> int {
        if (p[u] == -1) return 0;
        return 1 + self(self, p[u]);
    };
    while (true) {
        long long best_incr = -1;
        int best_r1 = -1, best_w = -1;
        for (int r1 : active_roots) {
            for (int w : adj[r1]) {
                int root_w = find_root(find_root, w);
                if (root_w == r1) continue;
                int d_w = compute_depth(compute_depth, w);
                long long incr = (long long)(d_w + 1) * comp_sum[r1];
                int new_mh = max(comp_mh[root_w], d_w + 1 + comp_mh[r1]);
                if (new_mh <= H && incr > best_incr) {
                    best_incr = incr;
                    best_r1 = r1;
                    best_w = w;
                }
            }
        }
        if (best_incr == -1) break;
        int root_w = find_root(find_root, best_w);
        int d_w = compute_depth(compute_depth, best_w);
        p[best_r1] = best_w;
        children[best_w].push_back(best_r1);
        comp_sum[root_w] += comp_sum[best_r1];
        comp_mh[root_w] = max(comp_mh[root_w], d_w + 1 + comp_mh[best_r1]);
        active_roots.erase(best_r1);
    }
    for (int i = 0; i < N; i++) {
        if (i > 0) cout << " ";
        cout << p[i];
    }
    cout << endl;
    return 0;
}