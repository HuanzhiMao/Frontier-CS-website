#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, r;
    DSU(int n = 0) { init(n); }
    void init(int n) {
        p.resize(n + 1);
        r.assign(n + 1, 0);
        iota(p.begin(), p.end(), 0);
    }
    int find(int x) {
        if (p[x] == x) return x;
        return p[x] = find(p[x]);
    }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (r[a] < r[b]) swap(a, b);
        p[b] = a;
        if (r[a] == r[b]) r[a]++;
        return true;
    }
};

struct Edge {
    int u, v;
    long long w;
    bool operator<(Edge const& other) const {
        if (w != other.w) return w < other.w;
        if (u != other.u) return u < other.u;
        return v < other.v;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        if (!(cin >> n)) break;
        vector<Edge> edges;
        edges.reserve(1LL * n * (n - 1) / 2);
        for (int i = 1; i <= n; ++i) {
            for (int j = i + 1; j <= n; ++j) {
                long long d;
                cin >> d;
                edges.push_back({i, j, d});
            }
        }
        sort(edges.begin(), edges.end());
        DSU dsu(n);
        vector<Edge> ans;
        ans.reserve(n - 1);
        for (auto &e : edges) {
            if (dsu.unite(e.u, e.v)) {
                ans.push_back(e);
                if ((int)ans.size() == n - 1) break;
            }
        }
        cout << "!";
        for (auto &e : ans) {
            cout << " " << e.u << " " << e.v << " " << e.w;
        }
        cout << "\n";
        cout.flush();
    }
    return 0;
}