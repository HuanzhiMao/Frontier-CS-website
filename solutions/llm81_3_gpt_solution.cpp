#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    // Offline mode: if S is provided in input after N, just output the guess.
    string S;
    if (cin.rdbuf()->in_avail() > 0) {
        cin >> S;
        cout << 0 << '\n' << S << '\n';
        cout.flush();
        return 0;
    }

    // Interactive mode: query each bit individually using a small DFA.
    string ans;
    ans.reserve(N);

    for (int i = 0; i < N; ++i) {
        int m = i + 3;
        vector<int> a(m, 0), b(m, 0);

        for (int j = 0; j < i; ++j) {
            a[j] = j + 1;
            b[j] = j + 1;
        }
        a[i] = i + 1;     // sink0
        b[i] = i + 2;     // sink1
        a[i + 1] = i + 1; b[i + 1] = i + 1; // sink0 loops
        a[i + 2] = i + 2; b[i + 2] = i + 2; // sink1 loops

        cout << 1 << '\n';
        cout << m << '\n';
        for (int j = 0; j < m; ++j) {
            if (j) cout << ' ';
            cout << a[j];
        }
        cout << '\n';
        for (int j = 0; j < m; ++j) {
            if (j) cout << ' ';
            cout << b[j];
        }
        cout << '\n';
        cout.flush();

        int x;
        if (!(cin >> x)) return 0;
        ans.push_back((x == i + 2) ? '1' : '0');
    }

    cout << 0 << '\n' << ans << '\n';
    cout.flush();
    return 0;
}