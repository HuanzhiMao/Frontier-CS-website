#include <iostream>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long n;
    std::cin >> n;

    // Initialize with a valid BSU of length 1: a_1 = n.
    // The objective value is V = 1 * n = n.
    __int128 max_v = n;
    long long best_k = 1;
    long long best_c = n;

    // Iterate through possible lengths k.
    // k cannot exceed 63 since 2^63 > 10^18 > 10^12.
    for (int k = 2; k <= 63; ++k) {
        // Calculate the maximum possible base 'c' for this length 'k'.
        unsigned __int128 power_of_2_k_minus_1 = (unsigned __int128)1 << (k - 1);
        
        if (power_of_2_k_minus_1 > (unsigned __int128)n) {
            // For larger k, c would be 0.
            break;
        }
        
        long long c = n / (long long)power_of_2_k_minus_1;

        // Calculate the objective V = k * c * (2^k - 1).
        unsigned __int128 power_of_2_k = (unsigned __int128)1 << k;
        __int128 sum_a = (__int128)c * (power_of_2_k - 1);
        __int128 current_v = (__int128)k * sum_a;

        if (current_v > max_v) {
            max_v = current_v;
            best_k = k;
            best_c = c;
        }
    }
    
    // Output the best found sequence.
    std::cout << best_k << "\n";
    for (int i = 1; i <= best_k; ++i) {
        // The elements of the sequence are c, 2c, 4c, ...
        unsigned __int128 val = (unsigned __int128)best_c * ((unsigned __int128)1 << (i - 1));
        long long v_ll = (long long)val;
        std::cout << v_ll << (i == best_k ? "" : " ");
    }
    std::cout << "\n";

    return 0;
}