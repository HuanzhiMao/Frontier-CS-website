#include <bits/stdc++.h>
using namespace std;

struct Mask {
    uint64_t lo = 0, hi = 0;
};

static inline void set_bit(Mask &m, int b) {
    if (b < 64) m.lo |= (1ULL << b);
    else m.hi |= (1ULL << (b - 64));
}

static inline bool equal_mask(const Mask &a, const Mask &b) {
    return a.lo == b.lo && a.hi == b.hi;
}

static inline Mask or_mask(const Mask &a, const Mask &b) {
    Mask r;
    r.lo = a.lo | b.lo;
    r.hi = a.hi | b.hi;
    return r;
}

// splitmix64 PRNG
static inline uint64_t splitmix64(uint64_t &x) {
    x += 0x9e3779b97f4a7c15ULL;
    uint64_t z = x;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, H;
    if (!(cin >> R >> H)) return 0;

    const int N = 1000;
    int m = R;
    if (m <= 0) m = 1;

    // Generate a constant-weight random code for each position
    vector<Mask> code(N + 1);
    vector<vector<int>> tests(m);
    int w = max(4, m / 8); // constant weight per column

    for (int i = 1; i <= N; ++i) {
        uint64_t seed = 0x123456789abcdef0ULL ^ (uint64_t)i * 0x9e3779b97f4a7c15ULL;
        vector<char> used(m, 0);
        int cnt = 0;
        while (cnt < w) {
            int t = (int)(splitmix64(seed) % (uint64_t)m);
            if (!used[t]) {
                used[t] = 1;
                set_bit(code[i], t);
                tests[t].push_back(i);
                ++cnt;
            }
        }
    }

    // Send all robots (queries)
    for (int t = 0; t < m; ++t) {
        cout << "? " << tests[t].size();
        for (int x : tests[t]) cout << " " << x;
        cout << "\n";
        cout.flush();
    }

    // Get results once
    cout << "@\n";
    cout.flush();

    int L;
    if (!(cin >> L)) {
        // No interactor present; output a default answer
        cout << "! 1 1\n";
        cout.flush();
        return 0;
    }
    vector<int> ans(L);
    for (int i = 0; i < L; ++i) cin >> ans[i];

    // Build observed mask
    Mask y;
    for (int t = 0; t < min(L, m); ++t) {
        if (ans[t]) set_bit(y, t);
    }

    // Decode by enumerating all pairs (including equal)
    int bestA = -1, bestB = -1, cnt = 0;

    for (int i = 1; i <= N; ++i) {
        if (equal_mask(code[i], y)) {
            bestA = i; bestB = i; ++cnt;
        }
    }
    for (int i = 1; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            Mask u = or_mask(code[i], code[j]);
            if (equal_mask(u, y)) {
                bestA = i; bestB = j; ++cnt;
                if (cnt > 1) {
                    // Keep searching to count all candidates; final output will be ambiguous if cnt>1
                }
            }
        }
    }

    if (cnt >= 1) {
        cout << "! " << bestA << " " << bestB << "\n";
    } else {
        // Fallback in case no decoding possible (e.g., mismatched I/O)
        cout << "! 1 1\n";
    }
    cout.flush();
    return 0;
}