#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Solver {
    ll n;
    unordered_map<unsigned long long, int> cache;
    int query_count;

    Solver(ll n_): n(n_), query_count(0) {
        cache.reserve(1 << 15);
        cache.max_load_factor(0.7);
    }

    inline unsigned long long key(ll a, ll b) {
        if (a > b) swap(a, b);
        return (unsigned long long)a << 32 | (unsigned long long)b;
    }

    int ask(ll x, ll y) {
        if (x == y) return 0;
        unsigned long long k = key(x, y);
        auto it = cache.find(k);
        if (it != cache.end()) return it->second;
        cout << "? " << x << " " << y << endl;
        cout.flush();
        int res;
        if (!(cin >> res)) exit(0);
        cache[k] = res;
        query_count++;
        return res;
    }

    inline ll baseDist(ll x, ll y) {
        ll d = llabs(x - y);
        return min(d, n - d);
    }

    inline ll cwDist(ll x, ll y) {
        ll d = y - x;
        d %= n;
        if (d < 0) d += n;
        return d;
    }

    inline ll idxFrom(ll x, ll steps) {
        ll t = (x - 1 + steps) % n;
        if (t < 0) t += n;
        return t + 1;
    }

    bool onShortestPath(ll s, ll t, ll z, int dst_st) {
        int ds_sz = ask(s, z);
        int dz_t = ask(z, t);
        return ds_sz + dz_t == dst_st;
    }

    // Find a pair (s,t) such that D(s,t) < baseDist(s,t)
    // Try multiple s spaced around the cycle, with t at opposite (floor(n/2) ahead)
    // If not found, try additional offsets.
    bool findImprovedPair(ll &s_out, ll &t_out, int &dst_st, ll &delta) {
        vector<ll> starts;
        int M = 64;
        for (int i = 0; i < M; i++) {
            ll s = 1 + ( (__int128)i * n / M );
            if (s > n) s -= n;
            starts.push_back(s);
        }
        // Also include some additional starts
        for (int i = 0; i < 32; i++) {
            ll s = 1 + ( (__int128)(i*1234567) % n );
            if (s > n) s -= n;
            starts.push_back(s);
        }
        // Deduplicate
        sort(starts.begin(), starts.end());
        starts.erase(unique(starts.begin(), starts.end()), starts.end());

        vector<int> offsets = {0, 1, -1, 2, -2, 3, -3, 4, -4};
        for (ll s : starts) {
            for (int off : offsets) {
                ll offset = (n/2) + off;
                if (offset < 1) continue;
                if (offset >= n) continue;
                ll t = idxFrom(s, offset);
                ll base = baseDist(s, t);
                int d = ask(s, t);
                if (d < base) {
                    s_out = s;
                    t_out = t;
                    dst_st = d;
                    delta = baseDist(s, t);
                    return true;
                }
            }
        }
        // Fallback: try more random pairs if not found
        std::mt19937_64 rng(712367);
        for (int tries = 0; tries < 200; tries++) {
            ll s = rng() % n + 1;
            ll t = idxFrom(s, (n/2) + (ll)(rng()%5) - 2);
            ll base = baseDist(s, t);
            int d = ask(s, t);
            if (d < base) {
                s_out = s;
                t_out = t;
                dst_st = d;
                delta = baseDist(s, t);
                return true;
            }
        }
        // As ultimate fallback: scan some more systematic starts
        for (ll s = 1; s <= min<ll>(n, 500); s++) {
            ll t = idxFrom(s, n/2);
            ll base = baseDist(s, t);
            int d = ask(s, t);
            if (d < base) {
                s_out = s;
                t_out = t;
                dst_st = d;
                delta = baseDist(s, t);
                return true;
            }
        }
        return false;
    }

    // Binary search for 'p' = last vertex along cw from s towards t that is on a shortest s-t path
    // and also satisfies D(s, z) == cw steps.
    ll findP_on_path(ll s, ll t, int dst_st, ll delta) {
        ll lo = 0, hi = delta;
        while (lo < hi) {
            ll mid = (lo + hi + 1) >> 1;
            ll z = idxFrom(s, mid);
            bool ok = false;
            if (onShortestPath(s, t, z, dst_st)) {
                int ds_sz = ask(s, z);
                if ((ll)ds_sz == mid) ok = true;
            }
            if (ok) lo = mid;
            else hi = mid - 1;
        }
        return idxFrom(s, lo);
    }

    // Find q as the unique vertex (non-adjacent) with D(p,q)=1.
    // We do convex/unimodal search on F(k) = D(p, idxFrom(p, k)) for k in [2, floor(n/2)].
    ll findQ_via_unimodal(ll p) {
        ll L = 2;
        ll R = n / 2; // floor
        if (L > R) {
            // n=4: L=2,R=2 still ok; but if n=4, L=2,R=2 works.
            L = R = n/2;
        }
        auto F = [&](ll k)->int{
            ll v = idxFrom(p, k);
            return ask(p, v);
        };
        // Discrete convex/uni-modal search using slope comparison
        while (L < R) {
            ll mid = (L + R) >> 1;
            int fmid = F(mid);
            int fmid1 = F(mid + 1);
            if (fmid <= fmid1) R = mid;
            else L = mid + 1;
        }
        ll q = idxFrom(p, L);
        // In principle F(L) should be 1 and L>=2 implies non-adjacent
        // Safety: if it's not 1 (pathological), try neighbors around L
        int fq = ask(p, q);
        if (fq != 1 || baseDist(p, q) == 1) {
            // Search small window around
            ll span = 10;
            ll bestk = -1;
            int bestv = INT_MAX;
            for (ll k = max<ll>(2, L - span); k <= min<ll>(R + span, n/2); k++) {
                ll v = idxFrom(p, k);
                int val = ask(p, v);
                if (val < bestv) {
                    bestv = val;
                    bestk = k;
                }
            }
            if (bestk != -1) q = idxFrom(p, bestk);
        }
        return q;
    }

    // Full solve for one test
    void solve_one() {
        ll s, t, delta;
        int dst_st;
        bool ok = findImprovedPair(s, t, dst_st, delta);
        if (!ok) {
            // Extremely unlikely: as a fallback pick two arbitrary points and try to infer.
            // We'll assume n is small-ish or judge won't put us here.
            // Just guess a random chord; but to avoid WA, we will try heuristic:
            // pick s=1, find p via scanning midpoints using binary method with artificial delta = baseDist(s, s+n/2)
            s = 1;
            t = idxFrom(s, n/2);
            dst_st = ask(s, t);
            delta = baseDist(s, t);
            if (dst_st >= delta) {
                // Force some other choice
                t = idxFrom(s, n/2 + 1);
                dst_st = ask(s, t);
                delta = baseDist(s, t);
            }
        }

        ll p = findP_on_path(s, t, dst_st, delta);
        ll q = findQ_via_unimodal(p);

        // Ensure q is not adjacent to p along cycle (if it is, swap attempt by searching opposite direction)
        if (baseDist(p, q) == 1) {
            // try alternative: search the other direction by reflecting k in [2, floor(n/2)]
            // pick the next best candidate
            ll bestk = -1;
            int bestv = INT_MAX;
            for (ll k = 2; k <= n/2; k++) {
                ll v = idxFrom(p, k);
                if (baseDist(p, v) == 1) continue;
                int val = ask(p, v);
                if (val < bestv) {
                    bestv = val;
                    bestk = k;
                }
            }
            if (bestk != -1) q = idxFrom(p, bestk);
        }

        // Output guess
        cout << "! " << p << " " << q << endl;
        cout.flush();
        int verdict;
        if (!(cin >> verdict)) exit(0);
        if (verdict == -1) exit(0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        long long n;
        cin >> n;
        Solver solver(n);
        solver.solve_one();
    }
    return 0;
}