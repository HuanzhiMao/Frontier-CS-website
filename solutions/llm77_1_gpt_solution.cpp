#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    // Process possibly multiple test cases until EOF
    while (cin >> n >> m) {
        vector<double> logw(n, 0.0); // log-weights
        const double eta = 0.2;      // learning rate for Hedge (controls (1+eta) factor)
        string s, outcome_str;

        // Fixed seed for reproducibility
        std::mt19937 rng(712367239);
        std::uniform_real_distribution<double> uni(0.0, 1.0);

        for (int t = 0; t < m; ++t) {
            if (!(cin >> s)) return 0;

            // Compute probability of predicting 1 using log-sum-exp for stability
            double a = logw[0];
            for (int i = 1; i < n; ++i) if (logw[i] > a) a = logw[i];

            double sum0 = 0.0, sum1 = 0.0;
            for (int i = 0; i < n; ++i) {
                double w = exp(logw[i] - a);
                if (s[i] == '1') sum1 += w;
                else sum0 += w;
            }

            double denom = sum0 + sum1;
            double p = (denom > 0 ? sum1 / denom : 0.5);

            // Randomized prediction according to p
            char outc = (uni(rng) < p ? '1' : '0');
            cout << outc << '\n' << flush;

            // Read actual outcome and update weights
            if (!(cin >> outcome_str)) return 0;
            int outcome = (outcome_str[0] == '1');

            for (int i = 0; i < n; ++i) {
                if ((s[i] == '1') != (outcome == 1)) {
                    logw[i] -= eta;
                }
            }
        }
    }

    return 0;
}