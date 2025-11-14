#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    const long long MOD = 1000000007LL;
    int n;
    if (!(cin >> n)) return 0;

    vector<int> ans(n, 0);
    long long suffixPlus = 0; // number of '+' in the suffix (i+1..n)

    for (int i = n; i >= 1; --i) {
        vector<long long> a(n + 1, 1);
        for (int j = 0; j < i; ++j) a[j] = 0; // force prefix value to 0
        a[i] = 1; // test value at position i
        // suffix values remain 1

        cout << "?";
        for (int j = 0; j <= n; ++j) cout << " " << a[j];
        cout << endl;
        cout.flush();

        long long R;
        if (!(cin >> R)) return 0;

        long long K = suffixPlus % MOD;
        long long expectedMul = K;
        long long expectedPlus = (K + 1) % MOD;

        if (R == expectedPlus) {
            ans[i - 1] = 0; // '+'
            suffixPlus = (suffixPlus + 1) % MOD;
        } else {
            ans[i - 1] = 1; // '*'
        }
    }

    cout << "!";
    for (int i = 0; i < n; ++i) {
        cout << " " << ans[i];
    }
    cout << endl;
    cout.flush();

    return 0;
}