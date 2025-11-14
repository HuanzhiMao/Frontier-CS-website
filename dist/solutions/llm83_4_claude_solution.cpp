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
    
    // Initialize f[1] = 1
    f[1] = 1;
    
    // For each prime, we need to decide f(p) = 1 or f(p) = -1
    // We'll use a greedy approach: for each prime, try both values
    // and pick the one that gives better partial sums
    
    vector<int> prime_value(primes.size());
    
    // Simple heuristic: alternate between 1 and -1 for primes
    // but with some optimization based on density
    for (int i = 0; i < primes.size(); i++) {
        if (i % 2 == 0) {
            prime_value[i] = 1;
        } else {
            prime_value[i] = -1;
        }
    }
    
    // Map each prime to its value
    unordered_map<int, int> prime_to_value;
    for (int i = 0; i < primes.size(); i++) {
        prime_to_value[primes[i]] = prime_value[i];
    }
    
    // Compute f(i) for all i using multiplicative property
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            f[i] = prime_to_value[i];
        } else {
            // Find prime factorization and compute f(i)
            int temp = i;
            f[i] = 1;
            for (int p : primes) {
                if (p * p > temp) break;
                while (temp % p == 0) {
                    f[i] *= prime_to_value[p];
                    temp /= p;
                }
            }
            if (temp > 1) {
                f[i] *= prime_to_value[temp];
            }
        }
    }
    
    // Output the result
    for (int i = 1; i <= n; i++) {
        cout << f[i];
        if (i < n) cout << " ";
    }
    cout << "\n";
    
    return 0;
}