#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        if (n <= 1) {
            cout << "!\n";
            continue;
        }
        long long m = 1LL * n * (n - 1) / 2;
        vector<long long> tri(m);
        for (long long i = 0; i < m; ++i) cin >> tri[i];

        // Precompute base indices for triangular indexing (1-based nodes)
        vector<long long> base(n + 2, 0);
        for (int i = 2; i <= n; ++i) base[i] = base[i - 1] + (n - (i - 1));

        auto getd = [&](int i, int j) -> long long {
            if (i == j) return 0;
            if (i > j) swap(i, j);
            // i < j
            return tri[ base[i] + (j - i - 1) ];
        };

        vector<long long> d1(n + 1, 0);
        for (int i = 2; i <= n; ++i) d1[i] = getd(1, i);

        vector<tuple<int,int,long long>> edges;
        edges.reserve(n - 1);

        for (int u = 2; u <= n; ++u) {
            long long bestDepth = -1;
            int par = -1;
            long long w = 0;
            for (int v = 1; v <= n; ++v) if (v != u) {
                if (d1[v] < d1[u]) {
                    long long duv = getd(u, v);
                    if (d1[v] + duv == d1[u]) {
                        if (d1[v] > bestDepth) {
                            bestDepth = d1[v];
                            par = v;
                            w = duv;
                        }
                    }
                }
            }
            if (par == -1) {
                // Fallback (should not happen for valid tree metrics): connect to 1
                par = 1;
                w = d1[u];
            }
            edges.emplace_back(par, u, w);
        }

        cout << "!";
        for (auto &e : edges) {
            int u, v;
            long long w;
            tie(u, v, w) = e;
            cout << " " << u << " " << v << " " << w;
        }
        cout << "\n";
    }
    return 0;
}