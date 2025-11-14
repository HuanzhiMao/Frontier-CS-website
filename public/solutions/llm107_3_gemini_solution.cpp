#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <set>
#include <map>

// Function to make a query and read the response
long long ask(long long Q) {
    std::cout << "0 " << Q << std::endl;
    long long response;
    std::cin >> response;
    return response;
}

// Function to submit the answer
void answer(long long ans) {
    std::cout << "1 " << ans << std::endl;
}

// Sieve to find primes up to a limit
std::vector<int> primes;
const int SIEVE_LIMIT = 40000;

void sieve() {
    std::vector<bool> is_prime_sieve(SIEVE_LIMIT + 1, true);
    is_prime_sieve[0] = is_prime_sieve[1] = false;
    for (int p = 2; p * p <= SIEVE_LIMIT; ++p) {
        if (is_prime_sieve[p]) {
            for (int i = p * p; i <= SIEVE_LIMIT; i += p)
                is_prime_sieve[i] = false;
        }
    }
    for (int p = 2; p <= SIEVE_LIMIT; ++p) {
        if (is_prime_sieve[p]) {
            primes.push_back(p);
        }
    }
}

std::vector<long long> Q_odd_primes;
std::vector<std::vector<int>> prime_groups;

void precompute_queries() {
    sieve();
    
    std::vector<int> current_group;
    unsigned __int128 current_q = 1;
    
    // Start from odd primes
    for (int p : primes) {
        if (p == 2) continue;
        if (Q_odd_primes.size() >= 25) break;

        if (current_q > (unsigned __int128)1000000000000000000ULL / p) {
            Q_odd_primes.push_back((long long)current_q);
            prime_groups.push_back(current_group);
            current_q = p;
            current_group = {p};
        } else {
            current_q *= p;
            current_group.push_back(p);
        }
    }
    if (current_q > 1 && Q_odd_primes.size() < 25) {
        Q_odd_primes.push_back((long long)current_q);
        prime_groups.push_back(current_group);
    }
}

void solve() {
    std::map<long long, int> prime_exponents;

    // Handle prime 2
    long long g = ask(1152921504606846976LL); // 2^60
    int a2 = 0;
    if (g > 0) {
        a2 = __builtin_ctzll(g);
    }
    prime_exponents[2] = a2;

    std::set<long long> found_odd_factors;
    for (size_t i = 0; i < Q_odd_primes.size(); ++i) {
        g = ask(Q_odd_primes[i]);
        if (g > 1) {
            for (int p : prime_groups[i]) {
                if (g % p == 0) {
                    found_odd_factors.insert(p);
                }
            }
        }
    }

    for (long long p : found_odd_factors) {
        unsigned __int128 Q_val = p;
        while (Q_val <= (unsigned __int128)1000000000000000000ULL / p) {
            Q_val *= p;
        }
        
        g = ask((long long)Q_val);
        int count = 0;
        if(g > 0) {
            while (g > 0 && g % p == 0) {
                count++;
                g /= p;
            }
        }
        prime_exponents[p] = count;
    }
    
    long long d_known = 1;
    for (auto const& [p, a] : prime_exponents) {
        d_known *= (a + 1);
    }

    answer(d_known * 2);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    precompute_queries();
    
    int T;
    std::cin >> T;
    while (T--) {
        solve();
    }

    return 0;
}