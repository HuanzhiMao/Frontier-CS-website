#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n = 1000000;
    vector<int> f(n + 1);
    vector<bool> is_prime(n + 1, true);
    vector<int> primes;
    
    // Sieve to find primes
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            for (long long j = (long long)i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    // Assign values to primes using greedy approach
    vector<int> prime_val(n + 1, 0);
    
    for (int p : primes) {
        vector<long long> sum_pos(n + 1, 0), sum_neg(n + 1, 0);
        
        // Calculate prefix sums if we assign +1 or -1 to this prime
        for (int i = 1; i <= n; i++) {
            if (i % p == 0) {
                int exp = 0;
                int temp = i;
                while (temp % p == 0) {
                    temp /= p;
                    exp++;
                }
                sum_pos[i] = sum_pos[i-1] + (exp % 2 == 1 ? 1 : -1) * (f[temp] != 0 ? f[temp] : 1);
                sum_neg[i] = sum_neg[i-1] + (exp % 2 == 1 ? -1 : 1) * (f[temp] != 0 ? f[temp] : 1);
            } else {
                sum_pos[i] = sum_pos[i-1] + (f[i] != 0 ? f[i] : 1);
                sum_neg[i] = sum_neg[i-1] + (f[i] != 0 ? f[i] : 1);
            }
        }
        
        long long max_pos = 0, max_neg = 0;
        for (int i = 1; i <= n; i++) {
            max_pos = max(max_pos, abs(sum_pos[i]));
            max_neg = max(max_neg, abs(sum_neg[i]));
        }
        
        prime_val[p] = (max_pos <= max_neg) ? 1 : -1;
    }
    
    // Construct f using multiplicative property
    f[1] = 1;
    for (int i = 2; i <= n; i++) {
        f[i] = 1;
        int temp = i;
        for (int p : primes) {
            if (p * p > temp) break;
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (exp % 2 == 1) {
                f[i] *= prime_val[p];
            }
        }
        if (temp > 1) { // temp is a prime
            f[i] *= prime_val[temp];
        }
    }
    
    for (int i = 1; i <= n; i++) {
        cout << f[i];
        if (i < n) cout << " ";
    }
    cout << "\n";
    
    return 0;
}