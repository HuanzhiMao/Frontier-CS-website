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

    int num_masks = 1 << m;
    std::vector<int> c(num_masks, -1);
    std::vector<std::string> M(num_masks);

    // Base cases for DP: masks with a single bit set
    for (int i = 0; i < m; ++i) {
        int mask = 1 << i;
        M[mask] = s[i];
        int current_c = 0;
        for (char ch : s[i]) {
            if (ch != '?') {
                current_c++;
            }
        }
        c[mask] = current_c;
    }

    // DP to compute merged patterns and their non-'?' counts
    for (int mask = 1; mask < num_masks; ++mask) {
        if (__builtin_popcount(mask) <= 1) {
            continue;
        }

        int lsb_idx = __builtin_ctz(mask);
        int prev_mask = mask ^ (1 << lsb_idx);
        
        if (c[prev_mask] == -1) {
            c[mask] = -1;
            continue;
        }

        const std::string& prev_M = M[prev_mask];
        const std::string& new_s = s[lsb_idx];
        
        M[mask].resize(n);
        int current_c = 0;
        bool conflict = false;

        for (int j = 0; j < n; ++j) {
            char char1 = prev_M[j];
            char char2 = new_s[j];

            if (char1 == '?') {
                M[mask][j] = char2;
            } else if (char2 == '?') {
                M[mask][j] = char1;
            } else if (char1 == char2) {
                M[mask][j] = char1;
            } else {
                conflict = true;
                break;
            }

            if (M[mask][j] != '?') {
                current_c++;
            }
        }

        if (conflict) {
            c[mask] = -1;
        } else {
            c[mask] = current_c;
        }
    }

    double probability = 0.0;
    
    // Inclusion-Exclusion Principle
    for (int mask = 1; mask < num_masks; ++mask) {
        if (c[mask] != -1) {
            double term = std::pow(0.25, c[mask]);
            if (__builtin_popcount(mask) % 2 == 1) { // Odd number of sets in intersection
                probability += term;
            } else { // Even number of sets
                probability -= term;
            }
        }
    }

    std::cout << std::fixed << std::setprecision(12) << probability << std::endl;

    return 0;
}