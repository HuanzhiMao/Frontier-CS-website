#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if(!(cin >> t)) return 0;
    const int N = 100000;

    for (int _ = 0; _ < t; ++_) {
        // Query 1: N ones
        cout << "? " << N;
        for (int i = 0; i < N; ++i) cout << " 1";
        cout << endl;
        cout.flush();

        long long l1;
        if(!(cin >> l1)) return 0;
        if (l1 == -1) return 0;

        if (l1 == 1) {
            cout << "! " << N << endl;
            cout.flush();
            continue;
        }

        long long LB = (N + l1 - 1) / l1;           // ceil(N / l1)
        long long UB = (N - 1) / (l1 - 1);          // floor((N - 1) / (l1 - 1))
        if (UB > 100000) UB = 100000;
        long long M = UB - LB;

        if (M == 0) {
            cout << "! " << LB << endl;
            cout.flush();
            continue;
        }

        long long n2 = 2 * M;
        cout << "? " << n2;
        for (long long i = 1; i <= M; ++i) {
            cout << " " << LB << " " << i;
        }
        cout << endl;
        cout.flush();

        long long L2;
        if(!(cin >> L2)) return 0;
        if (L2 == -1) return 0;

        long long r = 2 * M - L2;
        long long W = LB + r;

        cout << "! " << W << endl;
        cout.flush();
    }

    return 0;
}