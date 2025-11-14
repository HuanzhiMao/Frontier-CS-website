#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = (a >= 0 ? 1 : -1);
        y = 0;
        return llabs(a);
    }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

ll floor_div(ll a, ll b) { // floor(a/b) for any b != 0
    assert(b != 0);
    if (b < 0) { a = -a; b = -b; }
    if (a >= 0) return a / b;
    else return - ((-a + b - 1) / b);
}
ll ceil_div(ll a, ll b) { // ceil(a/b) for any b != 0
    assert(b != 0);
    if (b < 0) { a = -a; b = -b; }
    if (a >= 0) return (a + b - 1) / b;
    else return - (-a / b);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    if (!(cin >> N >> K)) {
        return 0;
    }
    vector<int> a(11);
    for (int i = 1; i <= 10; i++) cin >> a[i];
    vector<pair<int,int>> pts(N);
    for (int i = 0; i < N; i++) {
        int x, y; cin >> x >> y;
        pts[i] = {x, y};
    }

    // Parameters
    const int M = 100;            // Normal vector component range [-M, M]
    const long long Xlim = 1000000000LL; // Output coordinate bounds
    const double distMax = 9000.0; // Max distance from origin to line to ensure crossing circle (R=10000)

    // RNG
    unsigned long long seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937_64 rng(seed);
    uniform_int_distribution<int> distAB(-M, M);
    uniform_int_distribution<int> coin(0, 1);

    // For unique slopes (to reduce parallel lines)
    struct PairHash {
        size_t operator()(const pair<int,int>& p) const noexcept {
            return (static_cast<uint64_t>(static_cast<uint32_t>(p.first)) << 32) ^ static_cast<uint32_t>(p.second);
        }
    };
    unordered_set<pair<int,int>, PairHash> usedSlope;
    usedSlope.reserve(K * 4);

    vector<array<long long,4>> out;
    out.reserve(K);

    auto try_add_line = [&](long long A, long long B)->bool {
        if (A == 0 && B == 0) return false;
        long long g = std::gcd(llabs(A), llabs(B));
        long long Ap = A / g, Bp = B / g;

        // Enforce unique slopes (normalized)
        pair<int,int> key = {(int)Ap, (int)Bp};
        if (key.first < 0 || (key.first == 0 && key.second < 0)) {
            key.first = -key.first;
            key.second = -key.second;
            Ap = -Ap; Bp = -Bp;
            A = -A; B = -B;
        }
        if (usedSlope.find(key) != usedSlope.end()) return false;

        long double N2 = (long double)A * (long double)A + (long double)B * (long double)B;
        long double root = sqrt(N2);
        if (root <= 0) return false;
        long long S = (long long)floor(distMax * (double)root / (double)g);
        if (S <= 0) return false;

        long long smin = max(1LL, S / 50); // avoid too small offsets (avoid many lines through near-origin)
        if (smin > S) smin = S;

        std::uniform_int_distribution<long long> distS(smin, S);
        long long s = distS(rng) * (coin(rng) ? 1 : -1);
        if (s == 0) s = 1;

        // Avoid passing through any strawberry center: adjust s if necessary
        bool ok = false;
        long long C = 0;
        // Try nearby s if collision occurs
        for (int adj = 0; adj < 200; adj++) {
            if (adj > 0) {
                long long offs = (adj + 1) / 2;
                if (adj % 2 == 1) offs = -offs;
                long long cand = s + offs;
                if (llabs(cand) > S) continue;
                if (cand == 0) continue;
                s = cand;
            }
            C = g * s;
            bool coll = false;
            for (int i = 0; i < N; i++) {
                long long xi = pts[i].first, yi = pts[i].second;
                long long val = A * xi + B * yi;
                if (val == C) { coll = true; break; }
            }
            if (!coll) { ok = true; break; }
        }
        if (!ok) return false;

        // Find integer points on the line A x + B y = C
        long long u, v;
        long long gg = extgcd(A, B, u, v); // A*u + B*v = gg
        // gg should be equal to g (>0)
        if (gg == 0) return false;
        long long mult = s; // since C = g * s and gg == g
        long long x0 = u * mult;
        long long y0 = v * mult;

        // General solution: x = x0 + (B/g) * t, y = y0 - (A/g) * t
        long long d1 = Bp;      // coefficient for x
        long long d2 = -Ap;     // coefficient for y

        // Compute interval [l, r] such that both |x|<=Xlim and |y|<=Xlim
        long long l = LLONG_MIN / 4, r = LLONG_MAX / 4;

        if (d1 == 0) {
            if (llabs(x0) > Xlim) return false;
        } else {
            long long l1 = ceil_div(-Xlim - x0, d1);
            long long r1 = floor_div(Xlim - x0, d1);
            l = max(l, l1);
            r = min(r, r1);
        }
        if (d2 == 0) {
            if (llabs(y0) > Xlim) return false;
        } else {
            long long l2 = ceil_div(-Xlim - y0, d2);
            long long r2 = floor_div(Xlim - y0, d2);
            l = max(l, l2);
            r = min(r, r2);
        }
        if (l > r) return false;

        // Choose t1, t2 within [l, r] to get two distinct points
        long long t1;
        if (0 < l) t1 = l;
        else if (0 > r) t1 = r;
        else t1 = 0;

        long long t2;
        if (t1 + 1 <= r) t2 = t1 + 1;
        else if (t1 - 1 >= l) t2 = t1 - 1;
        else if (r - l >= 1) { t1 = l; t2 = r; }
        else return false; // cannot find two distinct points within bounds (very unlikely)

        long long px1 = x0 + d1 * t1;
        long long py1 = y0 + d2 * t1;
        long long px2 = x0 + d1 * t2;
        long long py2 = y0 + d2 * t2;

        if (px1 == px2 && py1 == py2) return false;
        if (!(llabs(px1) <= Xlim && llabs(py1) <= Xlim && llabs(px2) <= Xlim && llabs(py2) <= Xlim)) return false;

        // Finalize
        usedSlope.insert(key);
        out.push_back({px1, py1, px2, py2});
        return true;
    };

    int attempts = 0;
    int attemptLimit = 500000;
    while ((int)out.size() < K && attempts < attemptLimit) {
        attempts++;
        long long A = 0, B = 0;
        for (int t = 0; t < 10; t++) {
            A = distAB(rng);
            B = distAB(rng);
            if (A != 0 || B != 0) break;
        }
        if (A == 0 && B == 0) continue;
        if (try_add_line(A, B)) continue;
    }

    // If still not enough, relax uniqueness constraint by adding any random lines ignoring slope uniqueness
    // but still avoiding strawberries and bounds.
    auto try_add_line_relaxed = [&](long long A, long long B)->bool {
        if (A == 0 && B == 0) return false;
        long long g = std::gcd(llabs(A), llabs(B));
        long long Ap = A / g, Bp = B / g;

        long double N2 = (long double)A * (long double)A + (long double)B * (long double)B;
        long double root = sqrt(N2);
        if (root <= 0) return false;
        long long S = (long long)floor(distMax * (double)root / (double)g);
        if (S <= 0) return false;

        long long smin = max(1LL, S / 50);
        if (smin > S) smin = S;

        std::uniform_int_distribution<long long> distS(smin, S);
        long long s = distS(rng) * (coin(rng) ? 1 : -1);
        if (s == 0) s = 1;

        bool ok = false;
        long long C = 0;
        for (int adj = 0; adj < 200; adj++) {
            if (adj > 0) {
                long long offs = (adj + 1) / 2;
                if (adj % 2 == 1) offs = -offs;
                long long cand = s + offs;
                if (llabs(cand) > S) continue;
                if (cand == 0) continue;
                s = cand;
            }
            C = g * s;
            bool coll = false;
            for (int i = 0; i < N; i++) {
                long long xi = pts[i].first, yi = pts[i].second;
                long long val = A * xi + B * yi;
                if (val == C) { coll = true; break; }
            }
            if (!coll) { ok = true; break; }
        }
        if (!ok) return false;

        long long u, v;
        long long gg = extgcd(A, B, u, v);
        if (gg == 0) return false;
        long long mult = s;
        long long x0 = u * mult;
        long long y0 = v * mult;

        long long d1 = Bp;
        long long d2 = -Ap;

        long long l = LLONG_MIN / 4, r = LLONG_MAX / 4;
        if (d1 == 0) {
            if (llabs(x0) > Xlim) return false;
        } else {
            long long l1 = ceil_div(-Xlim - x0, d1);
            long long r1 = floor_div(Xlim - x0, d1);
            l = max(l, l1);
            r = min(r, r1);
        }
        if (d2 == 0) {
            if (llabs(y0) > Xlim) return false;
        } else {
            long long l2 = ceil_div(-Xlim - y0, d2);
            long long r2 = floor_div(Xlim - y0, d2);
            l = max(l, l2);
            r = min(r, r2);
        }
        if (l > r) return false;

        long long t1;
        if (0 < l) t1 = l;
        else if (0 > r) t1 = r;
        else t1 = 0;

        long long t2;
        if (t1 + 1 <= r) t2 = t1 + 1;
        else if (t1 - 1 >= l) t2 = t1 - 1;
        else if (r - l >= 1) { t1 = l; t2 = r; }
        else return false;

        long long px1 = x0 + d1 * t1;
        long long py1 = y0 + d2 * t1;
        long long px2 = x0 + d1 * t2;
        long long py2 = y0 + d2 * t2;

        if (px1 == px2 && py1 == py2) return false;
        if (!(llabs(px1) <= Xlim && llabs(py1) <= Xlim && llabs(px2) <= Xlim && llabs(py2) <= Xlim)) return false;

        out.push_back({px1, py1, px2, py2});
        return true;
    };

    int attempts2 = 0;
    while ((int)out.size() < K && attempts2 < 200000) {
        attempts2++;
        long long A = 0, B = 0;
        for (int t = 0; t < 10; t++) {
            A = distAB(rng);
            B = distAB(rng);
            if (A != 0 || B != 0) break;
        }
        if (A == 0 && B == 0) continue;
        if (try_add_line_relaxed(A, B)) continue;
    }

    // If still not enough, output what we have (possibly zero)
    cout << (int)out.size() << "\n";
    for (auto &ln : out) {
        cout << ln[0] << " " << ln[1] << " " << ln[2] << " " << ln[3] << "\n";
    }

    return 0;
}