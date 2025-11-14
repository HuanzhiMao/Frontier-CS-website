#include <iostream>
#include <vector>
#include <numeric>

const int n = 1000000;

int f[n + 1];
int min_prime[n + 1];

void sieve() {
    std::vector<int> primes;
    for (int i = 2; i <= n; ++i) {
        if (min_prime[i] == 0) {
            min_prime[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (p > min_prime[i] || (long long)i * p > n) {
                break;
            }
            min_prime[i * p] = p;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    sieve();

    f[1] = 1;
    long long current_sum = 1;

    for (int i = 2; i <= n; ++i) {
        if (min_prime[i] == i) { // i is prime
            if (current_sum > 0) {
                f[i] = -1;
            } else {
                f[i] = 1;
            }
        } else { // i is composite
            int p = min_prime[i];
            f[i] = f[p] * f[i / p];
        }
        current_sum += f[i];
    }

    for (int i = 1; i <= n; ++i) {
        std::cout << f[i] << (i == n ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}