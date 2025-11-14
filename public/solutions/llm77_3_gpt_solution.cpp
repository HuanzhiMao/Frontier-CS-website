#include <bits/stdc++.h>
using namespace std;

struct RNG {
    uint64_t x;
    RNG(uint64_t seed = 88172645463393265ull) : x(seed) {}
    uint64_t next() {
        x ^= x >> 12;
        x ^= x << 25;
        x ^= x >> 27;
        return x * 2685821657736338717ull;
    }
    double nextDouble() {
        return (next() >> 11) * (1.0 / 9007199254740992.0); // [0,1)
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    RNG rng(1234567891234567ull);
    const double eps = 0.1;
    const double multWrong = 1.0 - eps;

    while (cin >> n >> m) {
        vector<double> w(n, 1.0);
        double W = (double)n;

        for (int round = 0; round < m; ++round) {
            string s;
            if (!(cin >> s)) return 0;

            // Probability of predicting 1 is proportional to weights of experts predicting 1
            double sum1 = 0.0;
            for (int i = 0; i < n; ++i) {
                if (s[i] == '1') sum1 += w[i];
            }
            double p1 = (W > 0 ? sum1 / W : 0.5);

            char guess = (rng.nextDouble() < p1 ? '1' : '0');
            cout << guess << '\n';
            // No flush needed in non-interactive environment

            string outcome;
            if (!(cin >> outcome)) return 0;
            char y = outcome[0];

            // Update weights
            double sumWrong = 0.0;
            for (int i = 0; i < n; ++i) {
                if (s[i] != y) {
                    sumWrong += w[i];
                }
            }
            if (sumWrong > 0) {
                double factor = multWrong;
                for (int i = 0; i < n; ++i) {
                    if (s[i] != y) {
                        w[i] *= factor;
                    }
                }
                W -= eps * sumWrong;
                if (W < 1e-300) {
                    // Very unlikely, but in case of extreme underflow, renormalize
                    W = 0.0;
                    for (int i = 0; i < n; ++i) W += w[i];
                    if (W <= 0) {
                        // If all weights are zero (extreme edge), reset to uniform
                        for (int i = 0; i < n; ++i) w[i] = 1.0;
                        W = (double)n;
                    }
                }
            }
        }
    }
    return 0;
}