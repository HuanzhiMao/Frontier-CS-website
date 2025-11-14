#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

// Sieve to generate primes up to a certain limit
const int SIEVE_LIMIT = 1100000;
std::vector<int> primes;
bool is_prime[SIEVE_LIMIT + 1];

void sieve() {
    std::fill(is_prime, is_prime + SIEVE_LIMIT + 1, true);
    is_prime[0] = is_prime[1] = false;
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

// Helper to print a solution in the required format
void print_solution(const std::vector<long long>& a) {
    if (a.empty()) {
        // Fallback for cases where a construction yields no valid sequence
        // (e.g., n=1 for prime-based method).
        // Since n>=1, {1} is always a valid BSU of length 1.
        std::cout << 1 << "\n";
        std::cout << 1 << "\n";
        return;
    }
    std::cout << a.size() << "\n";
    for (size_t i = 0; i < a.size(); ++i) {
        std::cout << a[i] << (i == a.size() - 1 ? "" : " ");
    }
    std::cout << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    sieve();

    long long n;
    std::cin >> n;

    // Strategy 1: Powers of 2 (1, 2, 4, 8, ...)
    std::vector<long long> a_powers;
    if (n > 0) {
        long long current_power = 1;
        while (true) {
            a_powers.push_back(current_power);
            if (current_power > n / 2) {
                break;
            }
            current_power *= 2;
        }
    }

    __int128 v_powers = 0;
    if (!a_powers.empty()) {
        unsigned long long sum_powers = 0;
        for (long long val : a_powers) {
            sum_powers += val;
        }
        v_powers = (__int128)a_powers.size() * sum_powers;
    }

    // Strategy 2: Prime-based products (p1, p1*p2, p2*p3, ...)
    std::vector<long long> a_primes;
    if (!primes.empty() && (long long)primes[0] <= n) {
        a_primes.push_back(primes[0]);
        for (size_t i = 1; i < primes.size(); ++i) {
            if ((long long)primes[i - 1] > n / primes[i]) {
                break;
            }
            long long val = (long long)primes[i - 1] * primes[i];
            a_primes.push_back(val);
        }
    }
    
    __int128 v_primes = 0;
    if (!a_primes.empty()) {
        unsigned long long sum_primes = 0;
        for (long long val : a_primes) {
            sum_primes += val;
        }
        v_primes = (__int128)a_primes.size() * sum_primes;
    }
    
    if (v_primes > v_powers) {
        print_solution(a_primes);
    } else {
        print_solution(a_powers);
    }

    return 0;
}