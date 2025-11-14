#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        cin >> n;
        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
        }

        auto askAll = [&](void) -> long long {
            cout << "? 1 " << n;
            for (int i = 1; i <= n; ++i) cout << " " << i;
            cout << "\n" << flush;
            long long res;
            cin >> res;
            return res;
        };

        auto toggle = [&](int u) {
            cout << "? 2 " << u << "\n" << flush;
        };

        vector<long long> delta(n + 1, 0);
        long long R0 = askAll();
        long long Rprev = R0;
        long long sum_delta = 0;

        for (int i = 1; i <= n - 1; ++i) {
            toggle(i);
            long long Rnew = askAll();
            delta[i] = Rnew - Rprev;
            sum_delta += delta[i];
            Rprev = Rnew;
        }
        toggle(n);
        delta[n] = -2 * R0 - sum_delta;

        cout << "! ";
        for (int i = 1; i <= n; ++i) {
            int val = (delta[i] > 0 ? 1 : -1); // final values after our toggles
            if (i > 1) cout << " ";
            cout << val;
        }
        cout << "\n" << flush;
    }
    return 0;
}