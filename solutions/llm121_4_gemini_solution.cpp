#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

char combine(char c1, char c2) {
    if (c1 == 'X' || c2 == 'X') {
        return 'X';
    }
    if (c1 == '?') {
        return c2;
    }
    if (c2 == '?') {
        return c1;
    }
    if (c1 == c2) {
        return c1;
    }
    return 'X';
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m;
    std::vector<std::string> s(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> s[i];
    }

    int limit = 1 << m;
    std::vector<std::vector<char>> merged_char(limit, std::vector<char>(n));

    for (int mask = 1; mask < limit; ++mask) {
        int lsb_idx = __builtin_ctz(mask);
        int prev_mask = mask ^ (1 << lsb_idx);

        if (prev_mask == 0) { // Base case: mask has only one bit set
            for (int j = 0; j < n; ++j) {
                merged_char[mask][j] = s[lsb_idx][j];
            }
        } else { // Recursive step
            for (int j = 0; j < n; ++j) {
                merged_char[mask][j] = combine(merged_char[prev_mask][j], s[lsb_idx][j]);
            }
        }
    }

    std::vector<long double> power_of_025(n + 1);
    power_of_025[0] = 1.0L;
    for (int i = 1; i <= n; ++i) {
        power_of_025[i] = power_of_025[i - 1] * 0.25L;
    }

    long double total_prob = 0.0L;

    for (int mask = 1; mask < limit; ++mask) {
        int fixed_count = 0;
        bool incompatible = false;
        
        for (int j = 0; j < n; ++j) {
            if (merged_char[mask][j] == 'X') {
                incompatible = true;
                break;
            }
            if (merged_char[mask][j] != '?') {
                fixed_count++;
            }
        }

        if (incompatible) {
            continue;
        }

        long double term_prob = power_of_025[fixed_count];
        
        if (__builtin_popcount(mask) % 2 == 1) {
            total_prob += term_prob;
        } else {
            total_prob -= term_prob;
        }
    }

    std::cout << std::fixed << std::setprecision(12) << total_prob << std::endl;

    return 0;
}