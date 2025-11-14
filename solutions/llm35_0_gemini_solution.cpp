#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>

void solve() {
    int n;
    std::cin >> n;
    if (n == -1) {
        exit(0);
    }
    
    int num_indices = 2 * n - 1;
    if (num_indices <= 0) {
        // Should not happen with problem constraints, but as a safeguard.
        // If n=0, perhaps no answer is needed.
        // If n=1, array is [1], answer is 1. My code should handle this.
        if (n == 1) {
            std::cout << "! " << 1 << std::endl;
        }
        return;
    }

    int k_bits = std::floor(std::log2(num_indices)) + 1;
    if (num_indices == 1) k_bits = 1;


    std::vector<std::vector<int>> S(k_bits), S_prime(k_bits);
    for (int j = 1; j <= num_indices; ++j) {
        for (int i = 0; i < k_bits; ++i) {
            if (((j - 1) >> i) & 1) {
                S[i].push_back(j);
            } else {
                S_prime[i].push_back(j);
            }
        }
    }

    std::vector<int> candidates(n);
    std::iota(candidates.begin(), candidates.end(), 1);

    for (int i = 0; i < k_bits; ++i) {
        if (candidates.size() == 1) {
            break;
        }

        std::vector<int> next_candidates;
        std::vector<int>& s_indices = S[i];
        std::vector<int>& s_prime_indices = S_prime[i];

        for (int x : candidates) {
            int b_or, b_prime_or;
            
            if (s_indices.empty()) {
                b_or = 0;
            } else {
                std::cout << "? " << x << " " << s_indices.size();
                for (int idx : s_indices) {
                    std::cout << " " << idx;
                }
                std::cout << std::endl;
                std::cin >> b_or;
                if (b_or == -1) exit(0);
            }

            if (s_prime_indices.empty()) {
                b_prime_or = 0;
            } else {
                std::cout << "? " << x << " " << s_prime_indices.size();
                for (int idx : s_prime_indices) {
                    std::cout << " " << idx;
                }
                std::cout << std::endl;
                std::cin >> b_prime_or;
                if (b_prime_or == -1) exit(0);
            }
            
            int b_and = 1 - b_prime_or;

            if (b_or == b_and) {
                next_candidates.push_back(x);
            }
        }
        candidates = next_candidates;
    }

    std::cout << "! " << candidates[0] << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}