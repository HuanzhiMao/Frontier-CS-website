#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 1000000007LL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    auto query = [&](const vector<long long>& a) -> long long {
        cout << "?";
        for (long long x : a) {
            cout << " " << (x % MOD);
        }
        cout << "\n";
        cout.flush();
        long long res;
        if (!(cin >> res)) exit(0);
        return (res % MOD + MOD) % MOD;
    };

    const long long U = 2;      // a0
    const long long R = 2;      // change value for ak

    vector<int> op(n + 1, 0);   // 1..n used, 0 for '+', 1 for 'x'
    vector<long long> base(n + 1, 1);
    base[0] = U;

    long long y0 = query(base); // base query: all ones, a0 = U
    long long P_total = (y0 - U) % MOD;
    if (P_total < 0) P_total += MOD;

    int plus_count = 0;

    for (int k = 1; k <= n - 1; ++k) {
        vector<long long> a = base;
        a[k] = R;
        long long y = query(a);
        long long delta = (y - y0) % MOD;
        if (delta < 0) delta += MOD;

        long long S_prev = (U + plus_count) % MOD;

        if (delta == (R - 1) % MOD) {
            op[k] = 0; // '+'
            plus_count++;
        } else if (delta == (S_prev * ((R - 1) % MOD)) % MOD) {
            op[k] = 1; // 'x'
        } else {
            // Should not happen; fallback
            // Distinguish using S_prev != (R-1)
            if (S_prev == ((R - 1) % MOD)) {
                // ambiguous only if S_prev == R-1; but with U>=2 and R=2, S_prev>=2 and R-1=1, so not possible
                op[k] = 1;
            } else {
                // if delta == 1 -> plus else multiply
                op[k] = (delta == 1) ? 0 : 1;
                if (op[k] == 0) plus_count++;
            }
        }
    }

    // Determine last operator by total count of pluses
    long long P_total_int = P_total; // safe: P_total <= n < MOD
    if (plus_count < P_total_int) op[n] = 0; // '+'
    else op[n] = 1; // 'x'

    cout << "!";
    for (int i = 1; i <= n; ++i) {
        cout << " " << (op[i] == 0 ? 0 : 1);
    }
    cout << "\n";
    cout.flush();

    return 0;
}