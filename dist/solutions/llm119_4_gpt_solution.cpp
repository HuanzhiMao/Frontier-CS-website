#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 1000000007LL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;

    auto ask = [&](const vector<long long>& a)->long long{
        cout << "?";
        for (long long x : a) cout << " " << (x % MOD + MOD) % MOD;
        cout << endl;
        cout.flush();
        long long y;
        if (!(cin >> y)) exit(0);
        y %= MOD;
        if (y < 0) y += MOD;
        return y;
    };

    vector<long long> base(n + 1, 1);
    base[0] = 2; // a0 = 2, others initially 1

    // Perform n queries: for each i in 1..n, set ai = 2 (others as base)
    vector<long long> vals(n + 1, 0); // 1-based indexing for convenience
    vector<long long> v = base;
    for (int i = 1; i <= n; ++i) {
        v[i] = 2;
        vals[i] = ask(v);
        v[i] = 1;
    }

    vector<int> op(n, 0); // 0 for '+', 1 for 'x'

    bool allEqual = true;
    for (int i = 2; i <= n; ++i) if (vals[i] != vals[1]) { allEqual = false; break; }

    if (!allEqual) {
        // There is at least one '+', so min value = E_base + 1
        long long m = vals[1];
        for (int i = 2; i <= n; ++i) m = min(m, vals[i]);
        long long E_base = (m - 1 + MOD) % MOD; // small numbers, but keep modulo safe

        for (int i = 1; i <= n; ++i) {
            long long d = vals[i] - E_base;
            if (d < 0) d += MOD;
            if (d == 1) op[i - 1] = 0; // plus
            else op[i - 1] = 1;        // multiply
        }
    } else {
        // All values equal
        long long s = vals[1];
        if (n >= 2) {
            if (s == (3 + n)) {
                // All '+'
                for (int i = 0; i < n; ++i) op[i] = 0;
            } else if (s == 4) {
                // All 'x'
                for (int i = 0; i < n; ++i) op[i] = 1;
            } else {
                // Fallback (shouldn't happen): assume at least one '+'
                long long E_base = (s - 1 + MOD) % MOD;
                for (int i = 0; i < n; ++i) op[i] = 0;
            }
        } else {
            // n == 1: need an extra base query to disambiguate
            long long base_val = ask(base); // a0=2, a1=1
            long long d = vals[1] - base_val;
            if (d < 0) d += MOD;
            if (d == 1) op[0] = 0; // plus
            else op[0] = 1;        // multiply
        }
    }

    cout << "!";
    for (int i = 0; i < n; ++i) cout << " " << op[i];
    cout << endl;
    cout.flush();

    return 0;
}