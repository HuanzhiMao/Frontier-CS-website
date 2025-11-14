#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Using 128-bit integer for objective value to avoid overflow
using int128 = __int128_t;

const int PRIME_LIMIT = 1500000;
std::vector<int> primes;
std::vector<bool> is_prime;

void sieve(int limit) {
    is_prime.assign(limit + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (int i = p * p; i <= limit; i += p)
                is_prime[i] = false;
        }
    }
    for (int p = 2; p <= limit; ++p) {
        if (is_prime[p]) {
            primes.push_back(p);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long n;
    std::cin >> n;

    sieve(PRIME_LIMIT);

    std::vector<long long> best_seq;
    int128 best_V = 0;

    // Baseline: powers of 2
    if (n > 0) {
        best_seq.push_back(1);
        long long current_p2 = 1;
        long long p2_sum = 1;
        while (current_p2 * 2 <= n && best_seq.size() < 1000000) {
            current_p2 *= 2;
            best_seq.push_back(current_p2);
            p2_sum += current_p2;
        }
        best_V = (int128)best_seq.size() * p2_sum;
    }


    // Primes-based construction
    long long best_k_prime = 0;
    
    long long current_sum = 0;
    long long prev_ck = 0;

    for (int k = 3; k < primes.size() + 2; ++k) {
        if (k-2 >= primes.size() || k-3 >= primes.size()) break;
        long long p_km1 = primes[k-2]; // P_{k-1}
        long long p_km2 = primes[k-3]; // P_{k-2}

        if ((int128)p_km1 * p_km2 > n) {
            break;
        }

        long long c_max = n / p_km1;
        long long ck = c_max;
        if (ck % p_km2 == 0) {
            ck--;
        }

        if (ck <= p_km2) {
            break;
        }

        if (k == 3) {
            long long p1 = primes[0];
            long long p2 = primes[1];
            long long a1 = (long long)(p2 - 1) * p1;
            long long a2 = (long long)p1 * p2;
            long long a3 = (long long)ck * p2;
            current_sum = a1 + a2 + a3;
        } else {
            // S_k = S_{k-1} - c_{k-1}*p_{k-2} + p_{k-2}*p_{k-1} + c_k*p_{k-1}
            current_sum = current_sum - prev_ck * p_km2 + (long long)p_km2 * p_km1 + (long long)ck * p_km1;
        }
        
        prev_ck = ck;

        int128 current_V = (int128)k * current_sum;
        if (current_V > best_V) {
            best_V = current_V;
            best_k_prime = k;
        }
    }

    if (best_k_prime > 0) {
        best_seq.clear();
        long long p1 = primes[0];
        long long p2 = primes[1];
        best_seq.push_back((long long)(p2 - 1) * p1);
        
        for (int i = 2; i < best_k_prime; ++i) {
            best_seq.push_back((long long)primes[i-2] * primes[i-1]);
        }
        
        long long c_max = n / primes[best_k_prime - 2];
        long long ck = c_max;
        if (ck % primes[best_k_prime-3] == 0) {
            ck--;
        }
        best_seq.push_back((long long)ck * primes[best_k_prime-2]);
    }
    
    std::cout << best_seq.size() << "\n";
    for (size_t i = 0; i < best_seq.size(); ++i) {
        std::cout << best_seq[i] << (i == best_seq.size() - 1 ? "" : " ");
    }
    std::cout << "\n";

    return 0;
}