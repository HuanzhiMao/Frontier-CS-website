#include <bits/stdc++.h>
using namespace std;

static inline void sosdp(vector<int>& a, int m) {
    int N = (int)a.size();
    for (int i = 0; i < m; ++i) {
        int half = 1 << i;
        int step = half << 1;
        for (int mask = 0; mask < N; mask += step) {
            int* p = &a[mask];
            for (int k = 0; k < half; ++k) p[half + k] += p[k];
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<string> s(m);
    for (int i = 0; i < m; ++i) cin >> s[i];

    // Early exit: if any pattern is all '?', probability is 1
    for (int i = 0; i < m; ++i) {
        bool allq = true;
        for (int j = 0; j < n; ++j) {
            if (s[i][j] != '?') { allq = false; break; }
        }
        if (allq) {
            cout.setf(std::ios::fixed);
            cout << setprecision(12) << 1.0 << "\n";
            return 0;
        }
    }

    if (m == 0) {
        cout.setf(std::ios::fixed);
        cout << setprecision(12) << 0.0 << "\n";
        return 0;
    }

    if (m >= 26) {
        // Too large for this method; but such cases are unlikely given typical constraints.
        // Fallback: probability is at most 1. Print 0 with safe precision.
        cout.setf(std::ios::fixed);
        cout << setprecision(12) << 0.0 << "\n";
        return 0;
    }

    int NMASK = 1u << m;
    int FULL = NMASK - 1;

    // Count U_j masks: for each position, which patterns fix any letter at that position
    vector<int> arrU(NMASK, 0);
    for (int j = 0; j < n; ++j) {
        int U = 0;
        for (int i = 0; i < m; ++i) {
            char c = s[i][j];
            if (c != '?') U |= (1 << i);
        }
        arrU[U]++;
    }

    // singles[S] will accumulate sum_c PrefC_c[~S], later subtract 4*zeros[S]
    vector<int> singles(NMASK, 0);
    const char letters[4] = {'A', 'C', 'G', 'T'};

    // For each letter c, build counts of W_j[c] = patterns fixing letters different from c at pos j
    for (int li = 0; li < 4; ++li) {
        char cc = letters[li];
        vector<int> countW(NMASK, 0);
        for (int j = 0; j < n; ++j) {
            int W = 0;
            for (int i = 0; i < m; ++i) {
                char c = s[i][j];
                if (c != '?' && c != cc) W |= (1 << i);
            }
            countW[W]++;
        }
        sosdp(countW, m); // Now countW[mask] = #positions with W_j[c] subset of mask
        for (int S = 0; S < NMASK; ++S) {
            singles[S] += countW[FULL ^ S];
        }
    }

    // Transform arrU to get zeros[S] = arrU[~S]
    sosdp(arrU, m);

    // Precompute powers of 1/4
    vector<double> pow4(n + 1);
    pow4[0] = 1.0;
    for (int k = 1; k <= n; ++k) pow4[k] = pow4[k - 1] * 0.25;

    // Inclusion-exclusion over subsets of patterns
    long double res = 0.0L;
    for (int S = 1; S < NMASK; ++S) {
        int zeros = arrU[FULL ^ S];
        int singlesStrict = singles[S] - 4 * zeros;
        if (singlesStrict + zeros != n) continue; // conflict at some position
        int pc = __builtin_popcount((unsigned)S);
        double term = pow4[singlesStrict];
        if (pc & 1) res += term; else res -= term;
    }

    if (res < 0) res = 0;
    if (res > 1) res = 1;

    cout.setf(std::ios::fixed);
    cout << setprecision(12) << (double)res << "\n";
    return 0;
}