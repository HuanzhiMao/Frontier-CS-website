#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    string s(N, '0');
    for (int k = 0; k < N; ++k) {
        int m = k + 3;
        vector<int> a(m), b(m);

        for (int x = 0; x < k; ++x) {
            a[x] = x + 1;
            b[x] = x + 1;
        }
        a[k] = k + 1; // sink for '0'
        b[k] = k + 2; // sink for '1'
        a[k + 1] = k + 1; b[k + 1] = k + 1; // sink0 self-loop
        a[k + 2] = k + 2; b[k + 2] = k + 2; // sink1 self-loop

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
        cout << '\n';
        cout.flush();

        int x;
        if (!(cin >> x)) return 0;
        if (x == k + 1) s[k] = '0';
        else if (x == k + 2) s[k] = '1';
        else return 0; // unexpected
    }

    cout << 0 << ' ' << s << '\n';
    cout.flush();
    return 0;
}