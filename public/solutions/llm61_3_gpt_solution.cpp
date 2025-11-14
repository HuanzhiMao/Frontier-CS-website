#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int gain;
    int l, r;
    int ver; // version of l when this edge was created
    bool operator<(Edge const& other) const {
        return gain < other.gain; // max-heap
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int n, m;
        int c;
        cin >> n >> m >> c;
        vector<long long> a(n+1);
        for (int i = 1; i <= n; ++i) cin >> a[i];
        vector<long long> b(m+1), pref(m+1);
        for (int i = 1; i <= m; ++i) {
            cin >> b[i];
            pref[i] = pref[i-1] + b[i];
        }
        auto F = [&](long long x)->int{
            // number of k in [1..m] with pref[k] <= x
            return int(upper_bound(pref.begin()+1, pref.begin()+m+1, x) - (pref.begin()+1));
        };

        // Initialize segments
        vector<long long> sum(n+1);
        vector<int> fval(n+1);
        vector<int> L(n+1), R(n+1), ver(n+1, 0);
        vector<char> alive(n+1, 1);
        for (int i = 1; i <= n; ++i) {
            sum[i] = a[i];
            fval[i] = F(sum[i]);
            L[i] = i-1;
            R[i] = (i==n?0:i+1);
        }

        long long ans = 0;
        for (int i = 1; i <= n; ++i) ans += fval[i];
        ans -= 1LL * c * n;

        priority_queue<Edge> pq;
        auto push_edge = [&](int l, int r){
            if (l<=0 || r<=0) return;
            if (!alive[l] || !alive[r]) return;
            if (R[l] != r || L[r] != l) return;
            int g = c + F(sum[l] + sum[r]) - fval[l] - fval[r];
            if (g > 0) pq.push({g, l, r, ver[l]});
        };

        for (int i = 1; i < n; ++i) push_edge(i, i+1);

        while (!pq.empty()) {
            Edge e = pq.top(); pq.pop();
            int l = e.l, r = e.r;
            if (!alive[l]) continue;
            if (R[l] != r) continue;
            if (e.ver != ver[l]) continue;
            if (!alive[r]) continue; // redundant but safe

            // recompute to be safe (though should be same due to version)
            int g = c + F(sum[l] + sum[r]) - fval[l] - fval[r];
            if (g <= 0) continue;

            // perform merge l and r
            ans += g;
            sum[l] += sum[r];
            fval[l] = F(sum[l]);
            alive[r] = 0;
            ver[l]++;

            int rr = R[r];
            R[l] = rr;
            if (rr) L[rr] = l;

            // left neighbor edge update
            int ll = L[l];
            if (ll) push_edge(ll, l);
            // right neighbor edge update
            if (rr) push_edge(l, rr);
        }

        cout << ans << '\n';
    }
    return 0;
}