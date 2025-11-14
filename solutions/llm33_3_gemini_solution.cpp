#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <deque>
#include <map>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Using __int128_t for modular multiplication to avoid overflow.
using int128 = __int128_t;

// Modular exponentiation: (base^exp) % mod
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (int128)res * base % mod;
        base = (int128)base * base % mod;
        exp /= 2;
    }
    return res;
}

// Miller-Rabin primality test
bool miller_rabin(long long n, long long d) {
    long long a = 2 + rand() % (n - 3); // a in [2, n-2]
    long long x = power(a, d, n);
    if (x == 1 || x == n - 1) return true;
    while (d != n - 1) {
        x = (int128)x * x % n;
        d *= 2;
        if (x == 1) return false;
        if (x == n - 1) return true;
    }
    return false;
}

bool is_prime(long long n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    long long d = n - 1;
    while (d % 2 == 0) d /= 2;
    for (int i = 0; i < 8; i++) { // More iterations for higher confidence
        if (!miller_rabin(n, d)) return false;
    }
    return true;
}

long long gcd(long long a, long long b) {
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

// Pollard's rho algorithm for integer factorization
long long pollard_rho(long long n) {
    if (n % 2 == 0) return 2;
    if (is_prime(n)) return n;
    
    auto f = [&](long long x, long long c) {
        return ((int128)x * x + c) % n;
    };
    
    long long x = rand() % (n - 2) + 2;
    long long y = x;
    long long c = rand() % (n - 1) + 1;
    long long d = 1;
    
    while (d == 1) {
        x = f(x, c);
        y = f(f(y, c), c);
        d = gcd(std::abs(x - y), n);
        if (d == n) {
            // Failure, retry with new random values
            x = rand() % (n - 2) + 2;
            y = x;
            c = rand() % (n - 1) + 1;
            d = 1;
        }
    }
    return d;
}

// Find all prime factors of n
void factorize(long long n, std::vector<long long>& factors) {
    if (n <= 1) return;
    
    // Trial division for small primes
    while (n % 2 == 0) { factors.push_back(2); n /= 2; }
    while (n % 3 == 0) { factors.push_back(3); n /= 3; }
    
    if (n <= 1) return;
    if (is_prime(n)) {
        factors.push_back(n);
        return;
    }

    // For larger composite numbers, use Pollard's rho
    long long d = pollard_rho(n);
    factorize(d, factors);
    factorize(n / d, factors);
}

// Construct permutation for a number k using its binary representation
std::vector<int> construct_perm(long long k) {
    if (k == 1) return {};
    std::deque<int> p;
    int n = 0;
    
    int high_bit_idx = 0;
    if (k > 0) {
        high_bit_idx = 63 - __builtin_clzll(k);
    }
    
    for (int i = high_bit_idx - 1; i >= 0; --i) {
        p.push_back(n++); // Operation for 2*k
        if ((k >> i) & 1) {
            p.push_front(n++); // Operation for k+1
        }
    }
    return std::vector<int>(p.begin(), p.end());
}

void solve() {
    long long k;
    std::cin >> k;

    std::vector<long long> factors;
    factorize(k, factors);

    std::vector<int> total_perm;
    int current_offset = 0;

    for (long long p : factors) {
        std::vector<int> p_perm = construct_perm(p);
        for (int val : p_perm) {
            total_perm.push_back(val + current_offset);
        }
        current_offset += p_perm.size();
    }

    std::cout << total_perm.size() << "\n";
    for (size_t i = 0; i < total_perm.size(); ++i) {
        std::cout << total_perm[i] << (i == total_perm.size() - 1 ? "" : " ");
    }
    std::cout << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    srand(time(0));
    int q;
    std::cin >> q;
    while (q--) {
        solve();
    }
    return 0;
}