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

    long double total_prob = 0.0;

    for (int mask = 1; mask < (1 << m); ++mask) {
        int subset_size = __builtin_popcount(mask);

        int fixed_chars_count = 0;
        bool compatible = true;

        for (int j = 0; j < n; ++j) { // For each position in the string
            char merged_char = '?';
            for (int i = 0; i < m; ++i) {
                if ((mask >> i) & 1) { // If string i is in the current subset
                    if (s[i][j] != '?') {
                        if (merged_char == '?') {
                            merged_char = s[i][j];
                        } else if (merged_char != s[i][j]) {
                            compatible = false;
                            break;
                        }
                    }
                }
            }

            if (!compatible) {
                break;
            }

            if (merged_char != '?') {
                fixed_chars_count++;
            }
        }

        if (compatible) {
            long double term = powl(0.25L, fixed_chars_count);
            if (subset_size % 2 == 1) {
                total_prob += term;
            } else {
                total_prob -= term;
            }
        }
    }

    std::cout << std::fixed << std::setprecision(12) << total_prob << std::endl;

    return 0;
}