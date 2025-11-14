#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    while (cin >> n >> m) {
        vector<int> mistakes(n, 0);
        const long double eps = 0.5L;
        const long double lambda = -logl(1.0L - eps);
        const long double NINF = -numeric_limits<long double>::infinity();

        for (int round = 0; round < m; ++round) {
            string s;
            if (!(cin >> s)) return 0;

            bool has1 = false, has0 = false;
            long double max1 = NINF, max0 = NINF;

            for (int i = 0; i < n; ++i) {
                long double v = -lambda * (long double)mistakes[i];
                if (s[i] == '1') {
                    has1 = true;
                    if (v > max1) max1 = v;
                } else {
                    has0 = true;
                    if (v > max0) max0 = v;
                }
            }

            long double sumexp1 = 0.0L, sumexp0 = 0.0L;
            if (has1) {
                for (int i = 0; i < n; ++i) {
                    if (s[i] == '1') {
                        long double v = -lambda * (long double)mistakes[i];
                        sumexp1 += expl(v - max1);
                    }
                }
            }
            if (has0) {
                for (int i = 0; i < n; ++i) {
                    if (s[i] == '0') {
                        long double v = -lambda * (long double)mistakes[i];
                        sumexp0 += expl(v - max0);
                    }
                }
            }

            long double score1 = has1 ? (max1 + logl(sumexp1)) : NINF;
            long double score0 = has0 ? (max0 + logl(sumexp0)) : NINF;

            int guess = (score1 > score0) ? 1 : 0;
            cout << guess << '\n';
            cout.flush();

            string outcome;
            if (!(cin >> outcome)) return 0;
            char y = outcome[0];

            for (int i = 0; i < n; ++i) {
                if (s[i] != y) ++mistakes[i];
            }
        }
    }
    return 0;
}