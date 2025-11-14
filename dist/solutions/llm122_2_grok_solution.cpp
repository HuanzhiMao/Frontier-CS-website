#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    const int MAXW = 100000;
    for (int test = 0; test < t; test++) {
        int N = 100000;
        cout << "? " << N;
        for (int i = 0; i < N; i++) {
            cout << " 1";
        }
        cout << endl;
        int l;
        cin >> l;
        if (l == -1) exit(0);
        long long low, high;
        if (l == 1) {
            cout << "! " << MAXW << endl;
            continue;
        } else {
            low = (N + (long long)l - 1) / l;
            high = (N - 1LL) / (l - 1LL);
        }
        if (low == high) {
            cout << "! " << low << endl;
            continue;
        }
        int dd = high - low;
        cout << "? " << 2 * dd;
        for (int k = 1; k <= dd; k++) {
            cout << " " << low << " " << k;
        }
        cout << endl;
        int l2;
        cin >> l2;
        if (l2 == -1) exit(0);
        int extra = l2 - dd;
        int W = high - extra;
        cout << "! " << W << endl;
    }
    return 0;
}