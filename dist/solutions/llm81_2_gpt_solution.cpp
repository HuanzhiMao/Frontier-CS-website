#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    string S(N, '0');

    for (int pos = 0; pos < N; ++pos) {
        int m = pos + 3;
        vector<int> a(m, 0), b(m, 0);

        int s1 = pos + 1;
        int s0 = pos + 2;

        for (int t = 0; t <= pos - 1; ++t) {
            a[t] = t + 1;
            b[t] = t + 1;
        }

        a[pos] = s0;
        b[pos] = s1;

        a[s0] = s0; b[s0] = s0;
        a[s1] = s1; b[s1] = s1;

        cout << 1 << '\n';
        cout << m << '\n';
        for (int i = 0; i < m; ++i) {
            if (i) cout << ' ';
            cout << a[i];
        }
        cout << '\n';
        for (int i = 0; i < m; ++i) {
            if (i) cout << ' ';
            cout << b[i];
        }
        cout << '\n' << flush;

        int x;
        if (!(cin >> x)) return 0;

        if (x == s0) S[pos] = '0';
        else if (x == s1) S[pos] = '1';
        else {
            // Fallback (should not happen with correct interaction)
            S[pos] = '0';
        }
    }

    cout << 0 << '\n';
    cout << S << '\n' << flush;

    return 0;
}