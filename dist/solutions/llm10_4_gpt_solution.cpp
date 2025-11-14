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
        if (n <= 0) {
            cout << "!\n";
            continue;
        }
        long long m = 1LL * n * (n - 1) / 2;
        vector<long long> dtri(m);
        for (long long i = 0; i < m; ++i) {
            cin >> dtri[i];
        }
        // Precompute offsets for triangular indexing (0-based nodes)
        vector<long long> offset(n, 0);
        for (int i = 1; i < n; ++i) {
            offset[i] = offset[i-1] + (n - i);
        }
        auto getd = [&](int a, int b)->long long {
            if (a == b) return 0LL;
            if (a > b) swap(a, b);
            return dtri[offset[a] + (b - a - 1)];
        };

        // Prim's algorithm to reconstruct MST which equals the original tree
        const long long INF = (1LL<<62);
        vector<long long> best(n, INF);
        vector<int> parent(n, -1);
        vector<char> used(n, 0);
        best[0] = 0;

        for (int it = 0; it < n; ++it) {
            int u = -1;
            long long bv = INF;
            for (int i = 0; i < n; ++i) {
                if (!used[i] && best[i] < bv) {
                    bv = best[i];
                    u = i;
                }
            }
            if (u == -1) break;
            used[u] = 1;
            for (int v = 0; v < n; ++v) {
                if (!used[v]) {
                    long long w = getd(u, v);
                    if (w < best[v]) {
                        best[v] = w;
                        parent[v] = u;
                    }
                }
            }
        }

        cout << "!";
        for (int v = 1; v < n; ++v) {
            int u = parent[v];
            long long w = getd(u, v);
            cout << " " << (u + 1) << " " << (v + 1) << " " << w;
        }
        cout << "\n";
    }
    return 0;
}