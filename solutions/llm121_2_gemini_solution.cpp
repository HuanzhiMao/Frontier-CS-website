#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::string> s(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> s[i];
    }

    std::vector<long double> powers_of_025(n + 1);
    powers_of_025[0] = 1.0L;
    for (int i = 1; i <= n; ++i) {
        powers_of_025[i] = powers_of_025[i - 1] * 0.25L;
    }

    long double total_prob = 0;

    // Iterate through all 2^m - 1 non-empty subsets of patterns using a bitmask
    for (int mask = 1; mask < (1 << m); ++mask) {
        int constrained_chars = 0;
        bool possible = true;

        // For each character position, determine the merged character for the subset
        for (int j = 0; j < n; ++j) {
            char resolved_char = '?';
            for (int i = 0; i < m; ++i) {
                // Check if pattern 'i' is in the current subset
                if ((mask >> i) & 1) {
                    if (s[i][j] != '?') {
                        if (resolved_char == '?') {
                            resolved_char = s[i][j];
                        } else if (resolved_char != s[i][j]) {
                            // Conflict found, patterns in this subset are incompatible
                            possible = false;
                            break;
                        }
                    }
                }
            }
            if (!possible) {
                break;
            }
            if (resolved_char != '?') {
                constrained_chars++;
            }
        }

        if (possible) {
            // The probability of a random sequence matching the merged pattern
            long double term_prob = powers_of_025[constrained_chars];
            
            // Apply the Principle of Inclusion-Exclusion
            // Add for odd-sized subsets, subtract for even-sized ones
            if (__builtin_popcount(mask) % 2 == 1) {
                total_prob += term_prob;
            } else {
                total_prob -= term_prob;
            }
        }
    }

    std::cout << std::fixed << std::setprecision(12) << total_prob << std::endl;

    return 0;
}