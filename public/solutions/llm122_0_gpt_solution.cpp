#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;
    const int N = 100000;

    for (int _ = 0; _ < t; ++_) {
        // Query 1: N ones
        cout << "? " << N;
        for (int i = 0; i < N; ++i) cout << " 1";
        cout << endl;
        cout.flush();

        long long l1;
        if (!(cin >> l1)) return 0;
        if (l1 == -1) return 0;
        if (l1 == 0) return 0;

        long long A = (N + l1 - 1) / l1;

        // If l1 == 1, then W >= N but W <= 1e5, so W = N
        if (l1 == 1) {
            cout << "! " << N << endl;
            cout.flush();
            continue;
        }
        // If A == 1, then W must be 1 uniquely
        if (A == 1) {
            cout << "! 1" << endl;
            cout.flush();
            continue;
        }

        // Query 2: pairs (A, j) for j = 1..A-1
        int m = (int)A - 1;
        int n2 = 2 * m;
        cout << "? " << n2;
        for (int j = 1; j <= m; ++j) {
            cout << " " << A << " " << j;
        }
        cout << endl;
        cout.flush();

        long long l2;
        if (!(cin >> l2)) return 0;
        if (l2 == -1) return 0;
        if (l2 == 0) return 0;

        long long W = 3 * A - 2 - l2;
        cout << "! " << W << endl;
        cout.flush();
    }
    return 0;
}