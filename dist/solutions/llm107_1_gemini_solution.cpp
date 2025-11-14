#include <iostream>
#include <vector>
#include <numeric>
#include <set>
#include <cmath>
#include <algorithm>

using int128 = __int128;

void solve() {
    const int prime_limit = 1000;
    std::vector<int> primes;
    std::vector<bool> is_prime(prime_limit + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p <= prime_limit; ++p) {
        if (is_prime[p]) {
            primes.push_back(p);
            for (int i = p * p; i <= prime_limit; i += p)
                is_prime[i] = false;
        }
    }

    std::vector<long long> queries;
    std::vector<std::vector<int>> primes_in_query;
    if (!primes.empty()) {
        long long current_q = 1;
        std::vector<int> current_primes_group;
        for (int p : primes) {
            if (int128(current_q) * p > 1000000000000000000LL) {
                queries.push_back(current_q);
                primes_in_query.push_back(current_primes_group);
                current_q = p;
                current_primes_group = {p};
            } else {
                current_q *= p;
                current_primes_group.push_back(p);
            }
        }
        if (current_q > 1) {
            queries.push_back(current_q);
            primes_in_query.push_back(current_primes_group);
        }
    }

    std::set<int> found_primes;
    for (size_t i = 0; i < queries.size(); ++i) {
        std::cout << "0 " << queries[i] << std::endl;
        long long g;
        std::cin >> g;
        for (int p : primes_in_query[i]) {
            if (g % p == 0) {
                found_primes.insert(p);
            }
        }
    }

    long long d_1000 = 1;
    for (int p : found_primes) {
        long long q = p;
        long long max_X = 1000000000;
        
        while (q <= max_X / p) {
            q *= p;
        }
        q *= p;
        
        std::cout << "0 " << q << std::endl;
        long long g;
        std::cin >> g;
        
        int count = 0;
        if (g > 1) {
            while (g % p == 0) {
                count++;
                g /= p;
            }
        }
        d_1000 *= (count + 1);
    }
    
    long long ans = std::max(d_1000 + 7, 2 * d_1000);
    std::cout << "1 " << ans << std::endl;
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