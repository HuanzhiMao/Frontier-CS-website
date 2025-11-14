#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

// It's a competitive programming problem, so I'll follow the user's persona.
// No extra explanation, just the code.

long long N;
std::vector<long long> best_seq;
__int128_t max_V = 0;

void update_best(const std::vector<long long>& seq) {
    if (seq.empty()) return;
    __int128_t sum = 0;
    for (long long x : seq) {
        sum += x;
    }
    __int128_t current_V = (__int128_t)seq.size() * sum;
    if (current_V > max_V) {
        max_V = current_V;
        best_seq = seq;
    }
}

const int PRIME_LIMIT = 2000000;
std::vector<int> primes;
std::vector<bool> is_prime;

void sieve() {
    is_prime.assign(PRIME_LIMIT + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (is_prime[p]) {
            for (int i = p * p; i <= PRIME_LIMIT; i += p)
                is_prime[i] = false;
        }
    }
    for (int p = 2; p <= PRIME_LIMIT; ++p) {
        if (is_prime[p]) {
            primes.push_back(p);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cin >> N;

    // Strategy 1: Geometric progression family `a, b, 2b, 4b, ...`
    // We test for various small b, with a = b-1 which maximizes the sum for a fixed b.
    for (long long b = 2; b <= 300; ++b) {
        if (b > N) break;
        long long a = b - 1;
        
        std::vector<long long> seq;
        if (a > 0) seq.push_back(a);
        seq.push_back(b);

        long long current = b;
        while (true) {
            __int128_t next_val = (__int128_t)current * 2;
            if (next_val > N) {
                break;
            }
            current *= 2;
            seq.push_back(current);
        }
        update_best(seq);
    }
    
    // Specific case of geometric progression: 1, 2, 4, ...
    if (1 <= N) {
        std::vector<long long> seq124;
        seq124.push_back(1);
        long long current = 1;
        while(true){
            __int128_t next_val = (__int128_t)current * 2;
            if (next_val > N) break;
            current *= 2;
            seq124.push_back(current);
        }
        update_best(seq124);
    }


    // Strategy 2: Prime products `p_1, p_1*p_2, p_2*p_3, ...`
    sieve();
    if (!primes.empty() && primes[0] <= N) {
        std::vector<long long> prime_seq;
        prime_seq.push_back(primes[0]);
        for (size_t i = 1; i < primes.size(); ++i) {
            __int128_t next_val = (__int128_t)primes[i - 1] * primes[i];
            if (next_val > N) {
                break;
            }
            prime_seq.push_back(next_val);
        }
        update_best(prime_seq);
    }
    
    // Fallback for small N if no sequence is found
    if (best_seq.empty() && N > 0) {
        best_seq.push_back(1);
    }

    std::cout << best_seq.size() << "\n";
    for (size_t i = 0; i < best_seq.size(); ++i) {
        std::cout << best_seq[i] << (i == best_seq.size() - 1 ? "" : " ");
    }
    std::cout << "\n";

    return 0;
}