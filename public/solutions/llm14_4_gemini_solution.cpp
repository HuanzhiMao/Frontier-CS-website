#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

// Function to interact with the judge for a "walk" operation
long long query(long long x) {
    std::cout << "walk " << x << std::endl;
    long long result;
    std::cin >> result;
    if (result == -1) {
        exit(0); // Exit on judge error or invalid query
    }
    return result;
}

// Function to make a "guess"
void guess(long long g) {
    std::cout << "guess " << g << std::endl;
}

std::vector<int> primes;
// Max k is around sqrt(2 * 10^9) approx 44721. M is approx 10^9.
// To factor M, we need primes up to sqrt(M) approx 32000.
// To be safe, let's take it a bit higher. Max M is around k^2/2 where k(k+1)/2 >= 10^9.
// A sieve limit of 45000 is safe for factoring numbers up to 45000^2 > 2*10^9.
const int SIEVE_LIMIT = 45000;

void sieve() {
    std::vector<bool> is_prime(SIEVE_LIMIT + 1, true);
    if (SIEVE_LIMIT >= 0) is_prime[0] = false;
    if (SIEVE_LIMIT >= 1) is_prime[1] = false;
    for (int p = 2; p * p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            for (int i = p * p; i <= SIEVE_LIMIT; i += p)
                is_prime[i] = false;
        }
    }
    for (int p = 2; p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            primes.push_back(p);
        }
    }
}

// Function to get distinct prime factors of a number using pre-computed primes
std::vector<long long> get_distinct_prime_factors(long long n) {
    std::vector<long long> factors;
    long long temp_n = n;
    for (int p : primes) {
        if ((long long)p * p > temp_n) break;
        if (temp_n % p == 0) {
            factors.push_back(p);
            while (temp_n % p == 0) {
                temp_n /= p;
            }
        }
    }
    if (temp_n > 1) {
        factors.push_back(temp_n);
    }
    return factors;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    sieve();

    // Phase 1: Find a multiple of n
    long long s_vertex = query(0);

    long long M = -1;
    long long k_final = 0;
    for (long long k = 1; ; ++k) {
        long long current_v = query(k);
        if (current_v == s_vertex) {
            k_final = k;
            break;
        }
    }

    M = k_final * (k_final + 1) / 2;
    
    // Phase 2: Refine the multiple M to find n
    long long n_candidate = M;

    while (true) {
        if (n_candidate == 1) break;
        std::vector<long long> factors = get_distinct_prime_factors(n_candidate);
        bool changed = false;
        for (long long p : factors) {
            long long test_val = n_candidate / p;
            long long v = query(test_val);
            
            // Return to s_vertex to have a consistent state for the next test.
            // M is a multiple of n, so walking M steps returns to the same vertex.
            // We moved test_val from s, so walking M - test_val brings us back to s.
            query(M - test_val); 
            
            if (v == s_vertex) {
                n_candidate = test_val;
                changed = true;
                break; // Restart with the smaller candidate
            }
        }
        if (!changed) {
            break; // No more factors can be removed, n_candidate is n.
        }
    }

    guess(n_candidate);

    return 0;
}