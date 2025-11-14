#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 1000000007LL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    long long c = 2; // a0
    long long t = 2; // special value for aj
    vector<long long> a(n + 1, 1);
    a[0] = c;

    auto ask = [&](const vector<long long>& v) -> long long {
        cout << "?";
        for (int i = 0; i <= n; ++i) {
            cout << " " << v[i];
        }
        cout << endl;
        cout.flush();
        long long res;
        if (!(cin >> res)) exit(0);
        return res % MOD;
    };

    // Baseline query: a0 = c, all others = 1
    long long R0 = ask(a);

    vector<int> op(n + 1, 0); // 1..n, 0 for '+', 1 for '×'

    // Queries for j = 2..n with a_j = t
    for (int j = 2; j <= n; ++j) {
        a[j] = t;
        long long Rj = ask(a);
        a[j] = 1;
        long long delta = (Rj - R0) % MOD;
        if (delta < 0) delta += MOD;
        if (delta == (t - 1) % MOD) {
            op[j] = 0; // '+'
        } else {
            op[j] = 1; // '×'
        }
    }

    // Determine K = total number of '+'
    long long K = (R0 - c) % MOD;
    if (K < 0) K += MOD;
    if (K > n) K = (K % MOD); // K should be <= n; keep as is since MOD >> n

    int plus_after1 = 0;
    for (int j = 2; j <= n; ++j) if (op[j] == 0) ++plus_after1;

    // op1 is '+' if plus_after1 < K, else '×'
    if (plus_after1 < K) op[1] = 0; else op[1] = 1;

    // Output the operators
    cout << "!";
    for (int i = 1; i <= n; ++i) {
        cout << " " << (op[i] == 0 ? 0 : 1);
    }
    cout << endl;
    cout.flush();

    return 0;
}