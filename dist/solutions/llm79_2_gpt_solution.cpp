#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;
using i64 = long long;
using u128 = __uint128_t;

static inline u64 mul_mod(u64 a, u64 b, u64 mod) {
    return (u128)a * b % mod;
}
static inline int bits_len(u64 x) {
    if (x == 0) return 0;
    return 64 - __builtin_clzll(x);
}
static inline int weight_bits(u64 x) {
    return bits_len(x) + 1;
}

struct QueryData {
    u64 a;
    array<u64, 60> avals;   // a^(2^i) % n
    array<uint8_t, 60> w;   // bits(avals[i]) + 1
    i64 baseSquares;        // sum w^2
    i64 T;                  // measured time
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    u64 n;
    if (!(cin >> n)) return 0;

    auto ask = [&](u64 a) -> i64 {
        cout << "? " << a << endl;
        cout.flush();
        i64 t;
        if (!(cin >> t)) exit(0);
        return t;
    };

    // Special queries to get popcount(d) and d0 exactly
    // Query a = 1
    i64 T1 = ask(1);
    // bits(1)+1 = 2 for all steps, baseSquares = 60 * 4 = 240
    // extra per 1-bit = (bits(r)+1)*(2) with r=1 -> (1+1)*2 = 4
    // T1 = 240 + 4 * popcount(d)
    i64 k = (T1 - 240) / 4;
    if (k < 0) k = 0;

    // Query a = n-1 (i.e., -1 mod n)
    u64 aneg = (n == 0) ? 0 : (n - 1);
    i64 Tneg = ask(aneg);
    int w0 = weight_bits(aneg);
    i64 baseNeg = (i64)w0 * (i64)w0 + 59LL * 4LL;
    i64 Rneg = Tneg - baseNeg;
    int d0 = 0;
    if (k > 0) {
        if (Rneg == 4LL * k) d0 = 0;
        else d0 = 1; // Should be 2*w0*k otherwise
    } else {
        d0 = 0;
    }

    // Collect random queries
    // We will collect in batches and attempt reconstruction; increase if needed
    const int MAX_TOTAL_QUERIES = 29000 - 2; // leave space for final answer and above two queries
    int totalQueries = 0;

    vector<QueryData> data;
    data.reserve(30000);

    std::mt19937_64 rng((u64)chrono::high_resolution_clock::now().time_since_epoch().count() ^ (u64)(uintptr_t)new int);

    auto add_batch = [&](int B) {
        for (int t = 0; t < B; ++t) {
            if (totalQueries >= MAX_TOTAL_QUERIES) break;
            u64 a;
            // Avoid trivial a = 0, 1, n-1 to maintain variance
            for (;;) {
                u64 r = rng();
                if (n) r %= n;
                else r = 0;
                if (r != 0 && r != 1 && r != aneg) { a = r; break; }
            }
            QueryData q;
            q.a = a;
            u64 cur = a;
            i64 base = 0;
            for (int i = 0; i < 60; ++i) {
                q.avals[i] = cur;
                uint8_t w = (uint8_t)weight_bits(cur);
                q.w[i] = w;
                base += (i64)w * (i64)w;
                cur = mul_mod(cur, cur, n);
            }
            q.baseSquares = base;
            q.T = ask(a);
            data.push_back(q);
            ++totalQueries;
        }
    };

    // initial batch
    int BATCH = 4000;
    add_batch(BATCH);

    auto reconstruct = [&](vector<QueryData>& D, int d0, i64 k_known) -> pair<u64, bool> {
        int Q = (int)D.size();
        if (Q == 0) return { (u64)d0, false };

        vector<i64> R(Q);
        vector<u64> rcur(Q, 1);

        // Initialize residuals subtract base and bit 0 if needed
        for (int j = 0; j < Q; ++j) {
            R[j] = D[j].T - D[j].baseSquares;
            if (d0) {
                // contribution for i=0: (bits(1)+1)*w0
                R[j] -= (i64)(2) * (i64)D[j].w[0];
                // set rcur = a0
                rcur[j] = D[j].avals[0];
            } else {
                rcur[j] = 1;
            }
        }

        array<int,60> bitsd{};
        bitsd.fill(0);
        bitsd[0] = d0;

        vector<double> scores(60, 0.0);

        for (int i = 1; i < 60; ++i) {
            // Compute g_i^j = (bits(rcur[j])+1) * w[j][i]
            // Centered correlation
            long double sumG = 0.0L, sumR = 0.0L;
            vector<int> g(Q);
            for (int j = 0; j < Q; ++j) {
                int br = weight_bits(rcur[j]);
                int gij = br * (int)D[j].w[i];
                g[j] = gij;
                sumG += gij;
                sumR += (long double)R[j];
            }
            long double meanG = sumG / (long double)Q;
            long double meanR = sumR / (long double)Q;
            long double S = 0.0L, Den = 0.0L;
            for (int j = 0; j < Q; ++j) {
                long double gg = (long double)g[j] - meanG;
                long double rr = (long double)R[j] - meanR;
                S += rr * gg;
                Den += gg * gg;
            }
            int di = 0;
            double sc = 0.0;
            if (Den > 0) {
                long double ratio = S / Den;
                sc = (double)ratio;
                di = (S > Den * 0.5L) ? 1 : 0;
            } else {
                di = 0;
                sc = 0.0;
            }
            bitsd[i] = di;
            scores[i] = sc;

            if (di) {
                for (int j = 0; j < Q; ++j) {
                    R[j] -= (i64)g[j];
                    rcur[j] = mul_mod(rcur[j], D[j].avals[i], n);
                }
            }
        }

        // Build candidate d
        u64 dval = 0;
        int ones = 0;
        for (int i = 0; i < 60; ++i) {
            if (bitsd[i]) { dval |= (1ULL << i); ++ones; }
        }

        // Adjust popcount to match k_known if necessary
        if (k_known >= 0) {
            int targ = (int)k_known;
            if (ones != targ) {
                // Create list of indices with their confidence
                vector<pair<double,int>> idx;
                idx.reserve(60);
                for (int i = 1; i < 60; ++i) {
                    idx.emplace_back(scores[i], i);
                }
                if (ones > targ) {
                    // Turn off smallest confidence 1-bits
                    sort(idx.begin(), idx.end()); // ascending
                    for (auto &p : idx) {
                        if (ones == targ) break;
                        int i = p.second;
                        if (bitsd[i]) {
                            bitsd[i] = 0;
                            --ones;
                            dval &= ~(1ULL << i);
                        }
                    }
                } else {
                    // Turn on largest confidence 0-bits
                    sort(idx.begin(), idx.end(), greater<pair<double,int>>());
                    for (auto &p : idx) {
                        if (ones == targ) break;
                        int i = p.second;
                        if (!bitsd[i]) {
                            bitsd[i] = 1;
                            ++ones;
                            dval |= (1ULL << i);
                        }
                    }
                }
            }
        }

        // Verify by recomputing times
        bool ok = true;
        for (int j = 0; j < Q; ++j) {
            i64 res = D[j].baseSquares;
            u64 r = 1;
            for (int i = 0; i < 60; ++i) {
                if (bitsd[i]) {
                    res += (i64)weight_bits(r) * (i64)D[j].w[i];
                    r = mul_mod(r, D[j].avals[i], n);
                }
            }
            if (res != D[j].T) {
                ok = false;
                break;
            }
        }
        return { dval, ok };
    };

    u64 final_d = d0 ? 1ULL : 0ULL;
    bool solved = false;

    // Try with increasing batches
    for (int round = 0; round < 10 && !solved; ++round) {
        auto res = reconstruct(data, d0, k);
        if (res.second) {
            final_d = res.first;
            solved = true;
            break;
        }
        // Add more data if possible
        if (totalQueries >= MAX_TOTAL_QUERIES) break;
        add_batch(BATCH);
        // Optionally increase batch size gradually
        if (BATCH < 8000) BATCH += 2000;
    }

    if (!solved) {
        // Final attempt with current data anyway
        auto res = reconstruct(data, d0, k);
        final_d = res.first;
    }

    cout << "! " << final_d << endl;
    cout.flush();
    return 0;
}