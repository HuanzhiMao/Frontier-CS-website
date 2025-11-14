#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;
using u128 = __uint128_t;
using i64 = long long;

static inline int bits_u64(u64 x) {
    if (!x) return 0;
    return 64 - __builtin_clzll(x);
}
static inline int bp1(u64 x) {
    return bits_u64(x) + 1;
}
static inline u64 mul_mod_u64(u64 a, u64 b, u64 mod) {
    return (u128)a * (u128)b % (u128)mod;
}

struct RNG {
    u64 x;
    RNG() {
        u64 seed = chrono::high_resolution_clock::now().time_since_epoch().count();
        x = seed ^ (seed << 17) ^ (seed >> 7) ^ 0x9e3779b97f4a7c15ULL;
    }
    u64 next() {
        u64 z = (x += 0x9e3779b97f4a7c15ULL);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        return z ^ (z >> 31);
    }
    u64 uniform(u64 lim) { // [0, lim]
        return next() % (lim + 1);
    }
    u64 range(u64 l, u64 r) { // [l, r]
        return l + (next() % (r - l + 1));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    u64 n;
    if (!(cin >> n)) return 0;

    const int BITS = 60;
    const int MAX_QUERIES = 30000;

    // Storage for samples
    vector<u64> times_meas; times_meas.reserve(MAX_QUERIES);
    vector<u64> sum_squares; sum_squares.reserve(MAX_QUERIES);
    vector<u64> A; A.reserve((size_t)MAX_QUERIES * BITS); // flattened: index j*BITS + i
    vector<unsigned char> K; K.reserve((size_t)MAX_QUERIES * BITS); // bits(a_i)+1 per step

    RNG rng;

    auto add_samples = [&](int add_count) {
        for (int add = 0; add < add_count; ) {
            if ((int)times_meas.size() >= MAX_QUERIES) break;
            u64 a = rng.range(1, n - 1);
            // avoid gcd(a, n) != 1 to keep ai non-zero
            if (std::gcd(a, n) != 1) continue;
            // Avoid trivial a=1 to keep better signal
            if (a == 1) continue;

            // Precompute ai and K and sum of squares
            u64 x = a % n;
            u64 ssum = 0;
            size_t base = A.size();
            for (int i = 0; i < BITS; ++i) {
                A.push_back(x);
                unsigned char k = (unsigned char)bp1(x);
                K.push_back(k);
                ssum += (u64)k * (u64)k;
                x = mul_mod_u64(x, x, n);
            }
            sum_squares.push_back(ssum);

            cout << "? " << a << '\n' << flush;
            long long T;
            if (!(cin >> T)) return 0;
            times_meas.push_back((u64)T);
            ++add;
        }
    };

    auto decode = [&](vector<int>& bits_out) -> void {
        int Q = (int)times_meas.size();
        bits_out.assign(BITS, 0);
        vector<u64> r(Q, 1); // current r before step k
        vector<u64> pref(Q, 0); // contributions from bits < k
        vector<u32> D(Q); // cache D_j for step k

        for (int k = 0; k < BITS; ++k) {
            u128 denom = 0;
            u128 numer = 0;
            // compute D and residual E
            for (int j = 0; j < Q; ++j) {
                u64 rj = r[j];
                unsigned char kk = K[(size_t)j * BITS + k];
                u64 Dj = (u64)bp1(rj) * (u64)kk;
                D[j] = (u32)Dj;
                u64 Ej = times_meas[j] - sum_squares[j] - pref[j];
                denom += (u128)Dj * (u128)Dj;
                numer += (u128)Dj * (u128)Ej;
            }
            // decide bit based on regression coefficient ~ numer/denom
            int bit = ((numer << 1) > denom) ? 1 : 0;
            bits_out[k] = bit;

            if (bit) {
                // update pref and r
                for (int j = 0; j < Q; ++j) {
                    pref[j] += D[j];
                    u64 x = A[(size_t)j * BITS + k];
                    r[j] = mul_mod_u64(r[j], x, n);
                }
            }
        }
    };

    auto verify = [&](const vector<int>& bits_out) -> bool {
        int Q = (int)times_meas.size();
        for (int j = 0; j < Q; ++j) {
            u64 t = sum_squares[j];
            u64 r = 1;
            size_t base = (size_t)j * BITS;
            for (int i = 0; i < BITS; ++i) {
                if (bits_out[i]) {
                    u64 x = A[base + i];
                    t += (u64)bp1(r) * (u64)bp1(x);
                    r = mul_mod_u64(r, x, n);
                }
            }
            if (t != times_meas[j]) return false;
        }
        return true;
    };

    int total_queries = 0;
    // We'll add samples in increasing batches until verification passes or we hit limit.
    vector<int> bits_out;
    u64 d = 0;

    // Batch schedule
    int batch = 2000;
    bool ok = false;
    while (true) {
        int remaining = MAX_QUERIES - total_queries;
        if (remaining <= 0) break;
        int take = min(batch, remaining);
        add_samples(take);
        total_queries += take;

        decode(bits_out);
        // build d
        d = 0;
        for (int i = 0; i < BITS; ++i) if (bits_out[i]) d |= (1ULL << i);

        if (verify(bits_out)) { ok = true; break; }

        // Increase batch size for next round
        batch = min(batch * 2, 8000);
        // To avoid infinite loop, if very close to limit, attempt final decode
        if (total_queries >= MAX_QUERIES) break;
    }

    // As a last attempt, if not verified but we still have room, do one more big batch and retry
    if (!ok && total_queries < MAX_QUERIES) {
        int take = MAX_QUERIES - total_queries;
        if (take > 0) {
            add_samples(take);
            total_queries += take;
            decode(bits_out);
            d = 0;
            for (int i = 0; i < BITS; ++i) if (bits_out[i]) d |= (1ULL << i);
            ok = verify(bits_out);
        }
    }

    cout << "! " << d << '\n' << flush;
    return 0;
}