#include <bits/stdc++.h>
using namespace std;

struct DSU {
    int n, comps;
    vector<int> p, sz;
    DSU() {}
    DSU(int n_) { init(n_); }
    void init(int n_) {
        n = n_;
        p.resize(n);
        sz.assign(n, 1);
        iota(p.begin(), p.end(), 0);
        comps = n;
    }
    int find(int x) {
        while (p[x] != x) {
            p[x] = p[p[x]];
            x = p[x];
        }
        return x;
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        comps--;
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 400;
    const int M = 1995;

    vector<int> x(N), y(N);
    for (int i = 0; i < N; i++) {
        if (!(cin >> x[i] >> y[i])) return 0;
    }
    vector<int> U(M), V(M);
    for (int i = 0; i < M; i++) {
        cin >> U[i] >> V[i];
    }

    auto roundDist = [&](int a, int b) -> int {
        long long dx = x[a] - x[b];
        long long dy = y[a] - y[b];
        double d = sqrt((double)dx * dx + (double)dy * dy);
        return (int)floor(d + 0.5);
    };

    vector<int> D(M);
    for (int i = 0; i < M; i++) {
        D[i] = roundDist(U[i], V[i]);
        if (D[i] == 0) D[i] = 1; // safety (shouldn't happen)
    }

    // Compute an approximate target MST using D as weights over E
    vector<int> idx(M);
    iota(idx.begin(), idx.end(), 0);
    stable_sort(idx.begin(), idx.end(), [&](int a, int b){
        if (D[a] != D[b]) return D[a] < D[b];
        if (U[a] != U[b]) return U[a] < U[b];
        return V[a] < V[b];
    });
    vector<char> inApproxMST(M, 0);
    {
        DSU dsu(N);
        int cnt = 0;
        for (int e : idx) {
            if (dsu.unite(U[e], V[e])) {
                inApproxMST[e] = 1;
                cnt++;
                if (cnt == N - 1) break;
            }
        }
    }

    DSU accepted(N);

    auto canReject = [&](int iEdge) -> bool {
        // Check if accepted edges + all future edges (excluding current iEdge) can still connect the graph
        DSU tmp = accepted; // copy
        for (int j = iEdge + 1; j < M; j++) {
            tmp.unite(U[j], V[j]);
            if (tmp.comps == 1) return true; // early exit if already connected
        }
        return tmp.comps == 1;
    };

    for (int i = 0; i < M; i++) {
        int l;
        if (!(cin >> l)) return 0;

        int a = U[i], b = V[i];

        // If already connected in accepted forest, never take to avoid cycles
        if (accepted.find(a) == accepted.find(b)) {
            cout << 0 << endl;
            continue;
        }

        double p = (double)i / (double)M;
        double s = pow(p, 1.5);

        double thrNonStart = 1.26, thrNonEnd = 2.60;
        double thrTarStart = 1.48, thrTarEnd = 2.85;

        double thr = inApproxMST[i]
                     ? (thrTarStart + (thrTarEnd - thrTarStart) * s)
                     : (thrNonStart + (thrNonEnd - thrNonStart) * s);

        double ratio = (double)l / (double)D[i];

        bool take = false;
        if (ratio <= thr) {
            take = true;
        } else {
            // Safety check: ensure rejecting won't make future connectivity impossible
            if (!canReject(i)) take = true;
        }

        if (take) {
            accepted.unite(a, b);
            cout << 1 << endl;
        } else {
            cout << 0 << endl;
        }
    }

    return 0;
}