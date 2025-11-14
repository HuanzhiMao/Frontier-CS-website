#include <iostream>
#include <vector>
#include <numeric>

// Global precomputed data
std::vector<int> primes;
std::vector<std::vector<int>> prime_groups;

void sieve(int n) {
    std::vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (int i = p * p; i <= n; i += p)
                is_prime[i] = false;
        }
    }
    for (int p = 2; p <= n; ++p) {
        if (is_prime[p]) {
            primes.push_back(p);
        }
    }
}

void group_primes() {
    std::vector<int> current_group;
    __int128_t current_prod = 1;
    __int128_t limit = 1;
    for (int i = 0; i < 18; ++i) limit *= 10;

    for (int p : primes) {
        if (p > 0 && current_prod > limit / p) {
            prime_groups.push_back(current_group);
            current_group.clear();
            current_prod = 1;
        }
        current_group.push_back(p);
        current_prod *= p;
    }
    if (!current_group.empty()) {
        prime_groups.push_back(current_group);
    }
}

long long ask_query(long long Q) {
    std::cout << "0 " << Q << std::endl;
    long long response;
    std::cin >> response;
    return response;
}

void give_answer(long long ans) {
    std::cout << "1 " << ans << std::endl;
}

void solve() {
    long long d = 1;
    std::vector<int> found_factors;

    for (const auto& group : prime_groups) {
        long long Q = 1;
        for (int p : group) {
            Q *= p;
        }
        
        long long g = ask_query(Q);

        for (int p : group) {
            if (g % p == 0) {
                found_factors.push_back(p);
            }
        }
    }

    for (int p : found_factors) {
        long long p_ll = p;
        long long Q = p_ll;
        while (Q <= 1000000000LL / p_ll) {
            Q *= p_ll;
        }
        Q *= p_ll;
        
        long long g = ask_query(Q);
        int count = 0;
        if (g > 0) {
            while (g % p == 0) {
                g /= p;
                count++;
            }
        }
        d *= (count + 1);
    }
    
    give_answer(d * 2);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    sieve(1000);
    group_primes();

    int T;
    std::cin >> T;
    while (T--) {
        solve();
    }

    return 0;
}