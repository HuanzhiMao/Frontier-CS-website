#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;
using i64 = long long;
using u128 = unsigned __int128;

static const int B = 60;

static inline int bits64(u64 x) {
    return x ? (64 - __builtin_clzll(x)) : 0;
}

static inline u64 mul_mod(u64 a, u64 b, u64 mod) {
    return (u128)a * b % mod;
}

struct Sample {
    u64 a0;
    i64 time;
    array<u64, B> a;        // a at each iteration
    array<uint8_t, B> w;    // bits(a)+1 at each iteration
    uint32_t sum_sqr;       // sum of w^2 over 60 iterations
};

static i64 simulateTime(u64 a, u64 d, u64 n) {
    u64 r = 1;
    i64 t = 0;
    for (int i = 0; i < B; ++i) {
        int wa = bits64(a) + 1;
        if ((d >> i) & 1ULL) {
            int wr = bits64(r) + 1;
            t += (i64)wr * (i64)wa;
            r = mul_mod(r, a, n);
        }
        t += (i64)wa * (i64)wa;
        a = mul_mod(a, a, n);
    }
    return t;
}

static i64 ask(u64 a) {
    cout << "? " << a << endl;
    cout.flush();
    i64 t;
    if (!(cin >> t)) {
        exit(0);
    }
    return t;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    u64 n;
    if (!(cin >> n)) return 0;

    mt19937_64 rng((u64)chrono::high_resolution_clock::now().time_since_epoch().count());

    auto gen_a = [&](void)->u64 {
        u64 a;
        do {
            a = rng() % n;
        } while (a <= 1 || std::gcd(a, n) != 1ULL);
        return a;
    };

    // Query a=1 to get popcount(d)
    i64 t_one = ask(1);
    // S_sqr for a=1: each iteration bits(1)+1 = 2, so cost 4; 60 iterations: 240
    i64 popcnt_d = (t_one - 240) / 4;
    if (popcnt_d < 0) popcnt_d = 0;
    if (popcnt_d > 60) popcnt_d = 60;

    vector<Sample> samples;
    samples.reserve(30050);

    auto add_samples = [&](int cnt) {
        for (int it = 0; it < cnt; ++it) {
            u64 a0 = gen_a();
            Sample s;
            s.a0 = a0;
            s.sum_sqr = 0;
            u64 a = a0 % n;
            for (int i = 0; i < B; ++i) {
                s.a[i] = a;
                int w = bits64(a) + 1;
                s.w[i] = (uint8_t)w;
                s.sum_sqr += (uint32_t)(w * w);
                a = mul_mod(a, a, n);
            }
            s.time = ask(a0);
            samples.push_back(std::move(s));
        }
    };

    auto infer_d = [&](u64 n, const vector<Sample>& samples) -> pair<u64, vector<double>> {
        int L = (int)samples.size();
        vector<i64> residual(L);
        vector<u64> r(L, 1);
        for (int i = 0; i < L; ++i) {
            residual[i] = samples[i].time - samples[i].sum_sqr;
        }
        u64 d = 0;
        vector<double> slopes(B, 0.0);
        vector<int> c(L);
        for (int i = 0; i < B; ++i) {
            long double dot = 0.0L;
            long double norm = 0.0L;
            for (int j = 0; j < L; ++j) {
                int wr = bits64(r[j]) + 1;
                int ci = (int)samples[j].w[i] * wr;
                c[j] = ci;
                dot += (long double)ci * (long double)residual[j];
                norm += (long double)ci * (long double)ci;
            }
            double s = (norm > 0 ? (double)(dot / norm) : 0.0);
            slopes[i] = s;
            bool bit = s > 0.5;
            if (bit) {
                d |= (1ULL << i);
                for (int j = 0; j < L; ++j) {
                    residual[j] -= c[j];
                    r[j] = mul_mod(r[j], samples[j].a[i], n);
                }
            }
        }
        return {d, slopes};
    };

    auto verify_d = [&](u64 d, int cnt)->bool {
        for (int i = 0; i < cnt; ++i) {
            u64 a = gen_a();
            i64 t_meas = ask(a);
            i64 t_pred = simulateTime(a, d, n);
            if (t_meas != t_pred) return false;
        }
        return true;
    };

    int total_queries_used = 1; // already asked for a=1
    int initial = 8000;
    int step = 6000;
    int verify_cnt = 5;

    add_samples(initial);
    total_queries_used += initial;

    while (true) {
        auto res = infer_d(n, samples);
        u64 d = res.first;

        int pc = __builtin_popcountll(d);
        if (pc != popcnt_d) {
            int add = step;
            if (total_queries_used + add + verify_cnt >= 30000) add = max(0, 30000 - total_queries_used - verify_cnt);
            if (add <= 0) {
                // As a last chance, try to verify anyway
                if (verify_d(d, min(verify_cnt, 30000 - total_queries_used))) {
                    cout << "! " << d << endl;
                    cout.flush();
                    return 0;
                } else {
                    cout << "! " << d << endl;
                    cout.flush();
                    return 0;
                }
            }
            add_samples(add);
            total_queries_used += add;
            continue;
        }

        int vcnt = min(verify_cnt, 30000 - total_queries_used - 0);
        if (vcnt < 0) vcnt = 0;
        bool ok = true;
        if (vcnt > 0) {
            ok = verify_d(d, vcnt);
            total_queries_used += vcnt;
        }
        cout << "! " << d << endl;
        cout.flush();
        return 0;
    }
}