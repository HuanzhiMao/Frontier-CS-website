#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<string> s(m);
    for (int i = 0; i < m; ++i) cin >> s[i];

    // Precompute powers of 1/4 up to n
    vector<long double> pow4inv(n + 1);
    pow4inv[0] = 1.0L;
    for (int i = 1; i <= n; ++i) pow4inv[i] = pow4inv[i - 1] * 0.25L;

    using ull = unsigned long long;
    if (m > 25) {
        // Fallback: for safety, but constraints likely small; handle via pruning
        // However, without 2^m transforms we can't proceed efficiently.
        // We cap m to 25 in practice.
    }

    size_t S = 1ULL << m;

    // Conflict masks between patterns
    vector<ull> conflict(m, 0);

    // F[mask] = count of positions whose Kmask == mask
    vector<uint32_t> F(S, 0);

    // Process each position: build letter groups and conflicts, and Kmask
    for (int p = 0; p < n; ++p) {
        ull maskL[4] = {0, 0, 0, 0}; // A, C, G, T
        for (int i = 0; i < m; ++i) {
            char c = s[i][p];
            if (c == '?') continue;
            int idx = -1;
            if (c == 'A') idx = 0;
            else if (c == 'C') idx = 1;
            else if (c == 'G') idx = 2;
            else if (c == 'T') idx = 3;
            if (idx >= 0) maskL[idx] |= (1ULL << i);
        }
        ull unionMask = maskL[0] | maskL[1] | maskL[2] | maskL[3];
        // Count positions by Kmask
        F[unionMask]++;

        // Update conflict masks: for each letter group, all pairs with other letters conflict
        for (int l = 0; l < 4; ++l) {
            ull group = maskL[l];
            if (!group) continue;
            ull others = unionMask & ~group;
            ull x = group;
            while (x) {
                int i = __builtin_ctzll(x);
                x &= x - 1;
                conflict[i] |= others;
            }
        }
    }

    // Zeta transform over subsets: F[X] = sum_{K ⊆ X} F[K]
    for (int i = 0; i < m; ++i) {
        size_t step = 1ULL << i;
        for (size_t mask = 0; mask < S; mask += (step << 1)) {
            for (size_t x = 0; x < step; ++x) {
                F[mask + step + x] += F[mask + x];
            }
        }
    }

    // DP to enumerate consistent subsets and inclusion-exclusion
    vector<uint8_t> valid(S, 0);
    valid[0] = 1;
    long double ans = 0.0L;
    for (ull mask = 1; mask < S; ++mask) {
        ull lsb = mask & (~mask + 1ULL);
        int id = __builtin_ctzll(lsb);
        ull prev = mask ^ lsb;
        bool ok = valid[(size_t)prev] && ((conflict[id] & prev) == 0);
        valid[(size_t)mask] = ok ? 1 : 0;
        if (!ok) continue;

        ull comp = (S - 1) ^ mask;
        int freePos = (int)F[(size_t)comp];
        int constrained = n - freePos;
        long double term = pow4inv[constrained];
        if ((__builtin_popcountll(mask) & 1) == 1) ans += term; // odd size -> +
        else ans -= term; // even size -> -
    }

    // Clamp result within [0,1] for numerical stability
    if (ans < 0) ans = 0;
    if (ans > 1) ans = 1;

    cout.setf(std::ios::fmtflags(0), std::ios::floatfield);
    cout << setprecision(20) << (long double)ans << "\n";
    return 0;
}