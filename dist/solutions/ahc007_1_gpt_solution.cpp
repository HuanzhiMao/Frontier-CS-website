#include <bits/stdc++.h>
using namespace std;

struct DSU {
    int n;
    vector<int> p, sz;
    DSU(int n): n(n), p(n), sz(n,1) {
        iota(p.begin(), p.end(), 0);
    }
    int find(int x) {
        while (p[x] != x) {
            p[x] = p[p[x]];
            x = p[x];
        }
        return x;
    }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        return true;
    }
    bool same(int a, int b) { return find(a) == find(b); }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 400;
    const int M = 1995;

    vector<int> x(N), y(N);
    for (int i = 0; i < N; ++i) {
        if (!(cin >> x[i] >> y[i])) return 0;
    }

    vector<int> u(M), v(M);
    for (int i = 0; i < M; ++i) {
        if (!(cin >> u[i] >> v[i])) return 0;
    }

    DSU dsu(N);

    for (int i = 0; i < M; ++i) {
        int l;
        if (!(cin >> l)) return 0;
        int ans = 0;
        if (!dsu.same(u[i], v[i])) {
            ans = 1;
            dsu.unite(u[i], v[i]);
        }
        cout << ans << '\n' << flush;
    }

    return 0;
}