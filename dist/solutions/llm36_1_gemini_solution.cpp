#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

// Use __int128 for M to avoid overflow
using int128 = __int128;

long long query(const std::vector<long long>& q) {
    if (q.empty()) return 0;
    std::cout << 0 << " " << q.size();
    for (long long x : q) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
    long long collisions;
    std::cin >> collisions;
    return collisions;
}

void answer(long long n) {
    std::cout << 1 << " " << n << std::endl;
}

long long calculate_collisions(long long k, long long n) {
    if (n <= 1) return k * (k - 1) / 2;
    if (k < n) return 0;
    long long q = k / n;
    long long r = k % n;
    return r * q * (q + 1) / 2 + (n - r) * q * (q - 1) / 2;
}

const int P_LIMIT = 401;
std::vector<int> primes;

void sieve() {
    std::vector<bool> is_prime_sieve(P_LIMIT + 1, true);
    is_prime_sieve[0] = is_prime_sieve[1] = false;
    for (int p = 2; p * p <= P_LIMIT; ++p) {
        if (is_prime_sieve[p]) {
            for (int i = p * p; i <= P_LIMIT; i += p)
                is_prime_sieve[i] = false;
        }
    }
    for (int p = 2; p <= P_LIMIT; ++p) {
        if (is_prime_sieve[p]) {
            primes.push_back(p);
        }
    }
}

int128 M;

void compute_M() {
    M = 1;
    for (int p : primes) {
        if ((long double)M * p > 4e18) break; 
        M *= p;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    sieve();

    int k_small = 401;
    std::vector<long long> q_small(k_small);
    std::iota(q_small.begin(), q_small.end(), 1);

    long long c = query(q_small);

    if (c > 0) {
        for (long long n_cand = 2; n_cand < k_small; ++n_cand) {
            if (calculate_collisions(k_small, n_cand) == c) {
                answer(n_cand);
                return 0;
            }
        }
    }

    compute_M();

    std::vector<long long> q_M(k_small);
    for (int i = 0; i < k_small; ++i) {
        q_M[i] = (long long)(M * i) + 1;
    }
    long long c_m = query(q_M);

    long long F = 1;
    for (int p : primes) {
        int128 M_p = M / p;
        std::vector<long long> q_test(k_small);
        for (int i = 0; i < k_small; ++i) {
            q_test[i] = (long long)(M_p * i) + 1;
        }
        long long c_test = query(q_test);
        long long collisions_if_p_divides = calculate_collisions(k_small, p);

        if (c_test == collisions_if_p_divides) {
            F *= p;
            while ((__int128)F * p <= 1000000000LL) {
                 long long n_cand = F * p;
                 std::vector<long long> q_check = {1, 1 + n_cand};
                 if (query(q_check) == 1) {
                     F = n_cand;
                 } else {
                     break;
                 }
            }
        }
    }
    
    if (c_m > 0) {
        // m < k_small and m has no prime factors < P_LIMIT, so m must be 1.
        answer(F);
        return 0;
    }

    // C_m == 0 case -> m >= k_small
    long long low = 1, high = 1000000000LL / F, ans_m = 1;
    
    while(low <= high) {
        long long mid_m = low + (high-low)/2;
        if (mid_m == 0) { low=1; continue; }
        
        int128 test_d = (int128)F * mid_m;
        if (test_d > 2000000000000000000LL) { // Prevent overflow for the query numbers
             high = mid_m - 1;
             continue;
        }
        
        std::vector<long long> q_bs = {1, 1+(long long)test_d};
        if (query(q_bs) == 1) { // n | test_d => m_real | mid_m
            ans_m = mid_m;
            high = mid_m - 1;
        } else {
            low = mid_m + 1;
        }
    }
    answer(F * ans_m);

    return 0;
}