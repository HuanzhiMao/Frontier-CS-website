#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <numeric>

#if defined(__GNUC__) || defined(__clang__)
#define CTZ(x) __builtin_ctz(x)
#define POPCOUNT(x) __builtin_popcount(x)
#else
int get_ctz(int n) {
    int index = 0;
    while (!((n >> index) & 1)) {
        index++;
    }
    return index;
}
int get_popcount(int n) {
    int count = 0;
    while (n > 0) {
        n &= (n - 1);
        count++;
    }
    return count;
}
#define CTZ(x) get_ctz(x)
#define POPCOUNT(x) get_popcount(x)
#endif

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::string> s(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> s[i];
    }

    int num_masks = 1 << m;
    std::vector<std::string> merged_patterns(num_masks);
    std::vector<bool> is_possible(num_masks, false);
    std::vector<int> q_counts(num_masks, 0);

    is_possible[0] = true;
    merged_patterns[0] = std::string(n, '?');
    q_counts[0] = n;

    for (int mask = 1; mask < num_masks; ++mask) {
        int lsb_idx = CTZ(mask);
        int prev_mask = mask ^ (1 << lsb_idx);

        if (!is_possible[prev_mask]) {
            is_possible[mask] = false;
            continue;
        }

        std::string current_merged(n, ' ');
        bool possible_merge = true;
        int current_q_count = 0;

        for (int j = 0; j < n; ++j) {
            char c1 = merged_patterns[prev_mask][j];
            char c2 = s[lsb_idx][j];

            if (c1 == '?') {
                current_merged[j] = c2;
            } else if (c2 == '?') {
                current_merged[j] = c1;
            } else if (c1 == c2) {
                current_merged[j] = c1;
            } else {
                possible_merge = false;
                break;
            }
            if (current_merged[j] == '?') {
                current_q_count++;
            }
        }

        if (possible_merge) {
            is_possible[mask] = true;
            merged_patterns[mask] = current_merged;
            q_counts[mask] = current_q_count;
        } else {
            is_possible[mask] = false;
        }
    }

    double total_prob = 0.0;
    
    std::vector<double> pow_025(n + 1);
    pow_025[0] = 1.0;
    for (int i = 1; i <= n; ++i) {
        pow_025[i] = pow_025[i - 1] * 0.25;
    }

    for (int mask = 1; mask < num_masks; ++mask) {
        if (is_possible[mask]) {
            double prob = pow_025[n - q_counts[mask]];
            
            int pop_count = POPCOUNT(mask);

            if (pop_count % 2 == 1) {
                total_prob += prob;
            } else {
                total_prob -= prob;
            }
        }
    }

    std::cout << std::fixed << std::setprecision(12) << total_prob << std::endl;

    return 0;
}