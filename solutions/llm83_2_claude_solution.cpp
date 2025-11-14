#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n = 1000000;
    vector<int> f(n + 1);
    vector<bool> is_prime(n + 1, true);
    vector<int> primes;
    
    // Sieve to find all primes
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            for (long long j = (long long)i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    // Set f(1) = 1
    f[1] = 1;
    
    // For each prime, we need to assign f(p) = 1 or f(p) = -1
    // Use a greedy approach: for each prime, choose the value that minimizes the maximum prefix sum
    vector<int> prime_value(n + 1, 0);
    
    for (int p : primes) {
        // Try both values and see which gives better result
        vector<long long> prefix_sum_pos(n + 1, 0);
        vector<long long> prefix_sum_neg(n + 1, 0);
        
        // First set all prime values we've decided so far
        fill(f.begin(), f.end(), 0);
        f[1] = 1;
        
        for (int q : primes) {
            if (q > p) break;
            if (q == p) {
                prime_value[p] = 1; // temporarily try +1
                break;
            }
            if (prime_value[q] != 0) {
                continue;
            }
        }
        
        // Calculate f for all numbers using multiplicative property
        for (int i = 2; i <= n; i++) {
            if (is_prime[i]) {
                if (i == p) {
                    f[i] = 1; // trying +1 first
                } else if (prime_value[i] != 0) {
                    f[i] = prime_value[i];
                } else if (i > p) {
                    f[i] = 1; // default for unprocessed primes
                }
            } else {
                // Find prime factorization and multiply
                int temp = i;
                f[i] = 1;
                for (int q : primes) {
                    if (q * q > temp) break;
                    while (temp % q == 0) {
                        if (q == p) {
                            f[i] *= 1; // trying +1
                        } else if (prime_value[q] != 0) {
                            f[i] *= prime_value[q];
                        } else {
                            f[i] *= 1; // default
                        }
                        temp /= q;
                    }
                }
                if (temp > 1) {
                    if (temp == p) {
                        f[i] *= 1;
                    } else if (prime_value[temp] != 0) {
                        f[i] *= prime_value[temp];
                    } else {
                        f[i] *= 1;
                    }
                }
            }
        }
        
        // Calculate prefix sums for +1 case
        long long max_abs_pos = 0;
        for (int i = 1; i <= n; i++) {
            prefix_sum_pos[i] = prefix_sum_pos[i-1] + f[i];
            max_abs_pos = max(max_abs_pos, (long long)abs(prefix_sum_pos[i]));
        }
        
        // Now try -1 for prime p
        for (int i = 2; i <= n; i++) {
            if (is_prime[i]) {
                if (i == p) {
                    f[i] = -1; // trying -1 now
                } else if (prime_value[i] != 0) {
                    f[i] = prime_value[i];
                } else if (i > p) {
                    f[i] = 1;
                }
            } else {
                int temp = i;
                f[i] = 1;
                for (int q : primes) {
                    if (q * q > temp) break;
                    while (temp % q == 0) {
                        if (q == p) {
                            f[i] *= -1; // trying -1
                        } else if (prime_value[q] != 0) {
                            f[i] *= prime_value[q];
                        } else {
                            f[i] *= 1;
                        }
                        temp /= q;
                    }
                }
                if (temp > 1) {
                    if (temp == p) {
                        f[i] *= -1;
                    } else if (prime_value[temp] != 0) {
                        f[i] *= prime_value[temp];
                    } else {
                        f[i] *= 1;
                    }
                }
            }
        }
        
        long long max_abs_neg = 0;
        for (int i = 1; i <= n; i++) {
            prefix_sum_neg[i] = prefix_sum_neg[i-1] + f[i];
            max_abs_neg = max(max_abs_neg, (long long)abs(prefix_sum_neg[i]));
        }
        
        // Choose the better option
        if (max_abs_pos <= max_abs_neg) {
            prime_value[p] = 1;
        } else {
            prime_value[p] = -1;
        }
    }
    
    // Final calculation with chosen prime values
    f[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            f[i] = prime_value[i];
        } else {
            int temp = i;
            f[i] = 1;
            for (int p : primes) {
                if (p * p > temp) break;
                while (temp % p == 0) {
                    f[i] *= prime_value[p];
                    temp /= p;
                }
            }
            if (temp > 1) {
                f[i] *= prime_value[temp];
            }
        }
    }
    
    for (int i = 1; i <= n; i++) {
        cout << f[i];
        if (i < n) cout << " ";
    }
    cout << "\n";
    
    return 0;
}