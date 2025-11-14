#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cmath>

void solve() {
    int n, m;
    std::cin >> n >> m;

    std::vector<double> w_plus(n, 1.0);
    std::vector<double> w_minus(n, 1.0);

    const double beta = 0.5;

    for (int k = 0; k < m; ++k) {
        std::string s;
        std::cin >> s;

        double sum0 = 0.0;
        double sum1 = 0.0;

        for (int i = 0; i < n; ++i) {
            if (s[i] == '0') {
                sum0 += w_plus[i];
                sum1 += w_minus[i];
            } else {
                sum1 += w_plus[i];
                sum0 += w_minus[i];
            }
        }

        char my_guess;
        if (sum1 >= sum0) {
            my_guess = '1';
        } else {
            my_guess = '0';
        }

        std::cout << my_guess << std::endl;

        char outcome;
        std::cin >> outcome;

        double max_w = 0.0;
        for (int i = 0; i < n; ++i) {
            if (s[i] == outcome) {
                w_minus[i] *= beta;
            } else {
                w_plus[i] *= beta;
            }
            if (w_plus[i] > max_w) max_w = w_plus[i];
            if (w_minus[i] > max_w) max_w = w_minus[i];
        }
        
        if (max_w > 0 && max_w < 1e-50) {
            double scale_factor = 1e50;
            for (int i = 0; i < n; ++i) {
                w_plus[i] *= scale_factor;
                w_minus[i] *= scale_factor;
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}