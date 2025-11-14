#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>

// Function to calculate gcd, not really needed as std::gcd exists in C++17
// but implementing to be safe in older environments.
long long gcd(long long a, long long b) {
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

void solve() {
    // We will probe for small prime factors first.
    // X is at most 10^9. The product of the first 10 distinct primes (2*3*...*29)
    // already exceeds 10^9, so X has at most 9 distinct prime factors.
    // By querying for the first few primes, we can significantly constrain the structure of X.
    std::vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
    long long ans_part = 1;

    for (int p : primes) {
        // To find the exponent of a prime p in X, we can query with a large power of p.
        // If we query Q = p^k where p^k > 10^9, then gcd(X, p^k) will be exactly p^a,
        // where p^a is the p-part of X.
        __int128_t q_power = p;
        while (q_power * p <= 1000000000) {
            q_power *= p;
        }
        q_power *= p;
        
        long long q = (long long)q_power;
        std::cout << "0 " << q << std::endl;
        
        long long response;
        std::cin >> response;

        int count = 0;
        if (response > 1) {
            long long temp = response;
            while (temp % p == 0) {
                count++;
                temp /= p;
            }
        }
        ans_part *= (count + 1);
    }

    // After accounting for small prime factors, let X = G * Y, where G is the part we found
    // and Y is the remaining part. The number of divisors is d(X) = d(G) * d(Y).
    // Our calculated ans_part is d(G). The remaining part Y has only prime factors larger
    // than those we've tested.
    // The true number of divisors is ans_part * d(Y).
    // A good heuristic is needed to guess d(Y). d(Y)=1 (Y=1) or d(Y)=2 (Y is a large prime) are common cases.
    // Guessing ans = ans_part * 2 covers d(Y) in {1,2,3,4} for the relative error condition.
    // However, for small ans_part, absolute error is easier to satisfy.
    // A strategy that hedges between absolute and relative error is max(ans_part + 7, ans_part * 2).
    // ans_part + 7 is good if d(Y) is small, especially d(Y)=1.
    // ans_part * 2 is good if d(Y) is moderately small, and ans_part is larger.
    long long final_ans = std::max(ans_part + 7, ans_part * 2);

    std::cout << "1 " << final_ans << std::endl;
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