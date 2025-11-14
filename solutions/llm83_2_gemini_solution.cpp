#include <iostream>
#include <vector>
#include <numeric>

const int MAX_N = 1000000;

int lp[MAX_N + 1];
int f[MAX_N + 1];
std::vector<int> primes;

void linear_sieve() {
    for (int i = 2; i <= MAX_N; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (p > lp[i] || i * p > MAX_N) {
                break;
            }
            lp[i * p] = p;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    const int n = MAX_N;
    
    linear_sieve();
    
    f[1] = 1;
    long long current_sum = 1;
    
    for (int i = 2; i <= n; ++i) {
        if (lp[i] == i) { // i is prime
            if (current_sum > 0) {
                f[i] = -1;
            } else {
                f[i] = 1;
            }
        } else { // i is composite
            f[i] = f[lp[i]] * f[i / lp[i]];
        }
        current_sum += f[i];
    }
    
    for (int i = 1; i <= n; ++i) {
        std::cout << f[i] << (i == n ? "" : " ");
    }
    std::cout << "\n";
    
    return 0;
}