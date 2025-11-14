#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<string> s(m);
    for (auto &x : s) cin >> x;
    double ans = 0.0;
    for (int mask = 1; mask < (1 << m); mask++) {
        int bits = __builtin_popcount(mask);
        int sign = (bits & 1) ? 1 : -1;
        int f = 0;
        bool ok = true;
        for (int pos = 0; pos < n && ok; pos++) {
            char forced = 0;
            for (int i = 0; i < m; i++) {
                if (mask & (1 << i)) {
                    char c = s[i][pos];
                    if (c != '?') {
                        if (forced == 0) forced = c;
                        else if (forced != c) {
                            ok = false;
                            break;
                        }
                    }
                }
            }
            if (!ok) break;
            if (forced == 0) f++;
        }
        if (ok) {
            double contrib = sign * pow(0.25, n - f);
            ans += contrib;
        }
    }
    cout << fixed << setprecision(10) << ans << endl;
    return 0;
}