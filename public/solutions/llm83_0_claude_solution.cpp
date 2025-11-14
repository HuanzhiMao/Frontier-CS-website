#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n = 1000000;
    vector<int> f(n + 1);
    vector<int> spf(n + 1); // smallest prime factor
    vector<int> primes;
    
    // Sieve to find smallest prime factors
    for (int i = 1; i <= n; i++) spf[i] = i;
    for (int i = 2; i * i <= n; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j <= n; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    
    // Find all primes up to n
    for (int i = 2; i <= n; i++) {
        if (spf[i] == i) {
            primes.push_back(i);
        }
    }
    
    // Initialize f(1) = 1
    f[1] = 1;
    
    // For each prime, we need to assign f(p) = 1 or -1
    // We'll use a greedy approach to minimize the maximum partial sum
    map<int, int> prime_val;
    
    // Assign values to primes greedily
    for (int p : primes) {
        // Try both values and see which gives better result
        vector<long long> sum1, sum2;
        
        // Calculate partial sums up to current point with p = 1
        prime_val[p] = 1;
        long long s = 0;
        long long max_abs1 = 0;
        for (int i = 1; i <= min(n, p * 100); i++) {
            int val = 1;
            int temp = i;
            while (temp > 1) {
                int prime_factor = spf[temp];
                if (prime_val.count(prime_factor)) {
                    val *= prime_val[prime_factor];
                }
                while (temp % prime_factor == 0) {
                    temp /= prime_factor;
                }
            }
            s += val;
            max_abs1 = max(max_abs1, abs(s));
        }
        
        // Calculate with p = -1
        prime_val[p] = -1;
        s = 0;
        long long max_abs2 = 0;
        for (int i = 1; i <= min(n, p * 100); i++) {
            int val = 1;
            int temp = i;
            while (temp > 1) {
                int prime_factor = spf[temp];
                if (prime_val.count(prime_factor)) {
                    val *= prime_val[prime_factor];
                }
                while (temp % prime_factor == 0) {
                    temp /= prime_factor;
                }
            }
            s += val;
            max_abs2 = max(max_abs2, abs(s));
        }
        
        // Choose the better option
        if (max_abs1 <= max_abs2) {
            prime_val[p] = 1;
        } else {
            prime_val[p] = -1;
        }
    }
    
    // Simple alternating assignment for remaining primes
    int sign = 1;
    for (int p : primes) {
        if (!prime_val.count(p)) {
            prime_val[p] = sign;
            sign *= -1;
        }
    }
    
    // Calculate f(i) for all i using multiplicative property
    for (int i = 1; i <= n; i++) {
        f[i] = 1;
        int temp = i;
        while (temp > 1) {
            int p = spf[temp];
            f[i] *= prime_val[p];
            while (temp % p == 0) {
                temp /= p;
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