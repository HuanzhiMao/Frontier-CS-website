#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<string> s(m);
    for (int i = 0; i < m; ++i) cin >> s[i];
    
    int FULL = (m >= 31 ? -1 : ((1 << m) - 1));
    vector<int> adj(m, 0);
    int SZ = 1 << m;
    vector<int> freq(SZ, 0);
    
    // Process columns
    for (int j = 0; j < n; ++j) {
        int maskA = 0, maskC = 0, maskG = 0, maskT = 0, Fmask = 0;
        for (int i = 0; i < m; ++i) {
            char c = s[i][j];
            if (c == '?') continue;
            Fmask |= (1 << i);
            if (c == 'A') maskA |= (1 << i);
            else if (c == 'C') maskC |= (1 << i);
            else if (c == 'G') maskG |= (1 << i);
            else if (c == 'T') maskT |= (1 << i);
        }
        freq[Fmask]++;
        int unionAll = Fmask;
        int masks[4] = {maskA, maskC, maskG, maskT};
        for (int l = 0; l < 4; ++l) {
            int bL = masks[l];
            if (!bL) continue;
            int others = unionAll ^ bL;
            while (bL) {
                int i = __builtin_ctz(bL);
                bL &= bL - 1;
                adj[i] |= others;
            }
        }
    }
    
    // SOS DP: freq[mask] becomes sum over t subset mask of original freq[t]
    for (int i = 0; i < m; ++i) {
        for (int mask = 0; mask < SZ; ++mask) {
            if (mask & (1 << i)) freq[mask] += freq[mask ^ (1 << i)];
        }
    }
    
    // Precompute powers of 1/4
    vector<long double> pow4inv(n + 1, 1.0L);
    for (int i = 1; i <= n; ++i) pow4inv[i] = pow4inv[i - 1] * 0.25L;
    
    // DP for independent sets
    vector<unsigned char> indep(SZ, 0);
    indep[0] = 1;
    long double ans = 0.0L;
    int fullMask = (1 << m) - 1;
    for (int mask = 1; mask < SZ; ++mask) {
        int lsb = mask & -mask;
        int v = __builtin_ctz(lsb);
        int prev = mask ^ lsb;
        if (indep[prev] && ((adj[v] & prev) == 0)) {
            indep[mask] = 1;
            int c = n - freq[fullMask ^ mask];
            long double term = pow4inv[c];
            if (__builtin_popcount(mask) & 1) ans += term;
            else ans -= term;
        }
    }
    
    cout.setf(std::ios::fmtflags(0), std::ios::floatfield);
    cout << setprecision(15) << (double)ans << "\n";
    return 0;
}