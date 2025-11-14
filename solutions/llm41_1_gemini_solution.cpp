#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    long long n;
    std::cin >> n;

    unsigned __int128 max_v = 0;
    long long best_s = 1;
    int best_k = 1;

    if (n > 0) {
        max_v = 1;
    } else {
        // According to constraints 1 <= n, but for safety
        std::cout << 0 << "\n\n";
        return 0;
    }

    for (int p = 0; p < 63; ++p) {
        unsigned long long power_of_2_p = 1ULL << p;
        
        if (p > 0 && n < power_of_2_p) {
             break;
        }
        long long s_upper = n / power_of_2_p;
       
        long long s_lower = 0;
        if (p < 62) {
            s_lower = n / (1ULL << (p + 1));
        }
        
        long long s_cand = s_upper;
        if (s_cand % 2 == 0) {
            s_cand--;
        }
        
        if (s_cand > s_lower && s_cand > 0) {
            int k = p + 1;
            
            unsigned __int128 k_128 = k;
            unsigned __int128 s_128 = s_cand;
            
            unsigned __int128 sum_geom_series_128;
            if (k >= 128) {
                // This case is not reachable for n <= 10^12
                sum_geom_series_128 = -1; // Represents maximum unsigned value
            } else {
                sum_geom_series_128 = ((unsigned __int128)1 << k) - 1;
            }

            unsigned __int128 current_v = k_128 * s_128 * sum_geom_series_128;
            
            if (current_v > max_v) {
                max_v = current_v;
                best_s = s_cand;
                best_k = k;
            }
        }
    }

    std::cout << best_k << "\n";
    long long current_a = best_s;
    for (int i = 0; i < best_k; ++i) {
        std::cout << current_a << (i == best_k - 1 ? "" : " ");
        current_a *= 2;
    }
    std::cout << "\n";

    return 0;
}