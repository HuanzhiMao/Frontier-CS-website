#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    if (!(cin >> t)) return 0;
    const int M = 100000;
    while (t--) {
        // First query: n = M ones
        cout << "? " << M;
        for (int i = 0; i < M; ++i) cout << " 1";
        cout << "\n" << flush;

        long long l1;
        if (!(cin >> l1)) return 0;
        if (l1 == -1) return 0;

        if (l1 == 1) {
            // W must be M
            cout << "! " << M << "\n" << flush;
            continue;
        }

        long long L = (M + l1 - 1) / l1;
        long long U = (M - 1) / (l1 - 1);
        if (U > M) U = M; // clamp by given upper bound of W
        if (L > U) L = U; // safety

        long long B = U - L;
        if (B <= 0) {
            cout << "! " << L << "\n" << flush;
            continue;
        }

        // Second query: sequence of pairs [L, 1], [L, 2], ..., [L, B]
        long long n2 = 2 * B;
        cout << "? " << n2;
        for (long long i = 1; i <= B; ++i) {
            cout << " " << L << " " << i;
        }
        cout << "\n" << flush;

        long long l2;
        if (!(cin >> l2)) return 0;
        if (l2 == -1) return 0;

        long long g = 2 * B - l2; // g = W - L
        long long W = L + g;
        if (W < 1) W = 1;
        if (W > M) W = M;

        cout << "! " << W << "\n" << flush;
    }
    return 0;
}