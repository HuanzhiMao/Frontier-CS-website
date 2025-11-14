#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<string> s(m);
    for (int i = 0; i < m; i++) cin >> s[i];
    vector<long long> coeff(n + 1, 0LL);
    for (int mask = 1; mask < (1 << m); mask++) {
        int pop = __builtin_popcount(mask);
        long long sgn = (pop % 2 == 1 ? 1LL : -1LL);
        bool conf = false;
        int free_cnt = 0;
        for (int j = 0; j < n && !conf; j++) {
            char req = 0;
            bool has = false;
            for (int i = 0; i < m; i++) {
                if ((mask & (1 << i)) == 0) continue;
                char c = s[i][j];
                if (c != '?') {
                    if (has) {
                        if (c != req) {
                            conf = true;
                            break;
                        }
                    } else {
                        req = c;
                        has = true;
                    }
                }
            }
            if (!conf && !has) free_cnt++;
        }
        if (!conf) {
            coeff[free_cnt] += sgn;
        }
    }
    double prob = 0.0;
    for (int k = 0; k <= n; k++) {
        if (coeff[k] == 0) continue;
        prob += coeff[k] * pow(4.0, k - n);
    }
    cout << fixed << setprecision(10) << prob << endl;
    return 0;
}