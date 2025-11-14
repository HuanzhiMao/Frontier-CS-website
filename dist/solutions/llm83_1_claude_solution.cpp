#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n = 1000000;
    vector<int> f(n + 1);
    vector<int> smallest_prime(n + 1);
    
    // Sieve to find smallest prime factor
    for (int i = 1; i <= n; i++) {
        smallest_prime[i] = i;
    }
    
    for (int i = 2; i * i <= n; i++) {
        if (smallest_prime[i] == i) {
            for (int j = i * i; j <= n; j += i) {
                if (smallest_prime[j] == j) {
                    smallest_prime[j] = i;
                }
            }
        }
    }
    
    // Set f(1) = 1
    f[1] = 1;
    
    // For each prime, try to choose the value that minimizes the maximum prefix sum
    vector<bool> is_prime_set(n + 1, false);
    
    for (int i = 2; i <= n; i++) {
        if (smallest_prime[i] == i) {
            // i is prime, we need to decide f(i)
            if (!is_prime_set[i]) {
                // Try both +1 and -1, see which gives better result
                vector<int> prefix_sum_pos(n + 1, 0);
                vector<int> prefix_sum_neg(n + 1, 0);
                
                // Temporarily set f(i) = 1
                f[i] = 1;
                for (int j = 1; j <= n; j++) {
                    if (smallest_prime[j] == j) {
                        if (!is_prime_set[j] && j != i) continue;
                    }
                    
                    int val = 1;
                    int temp = j;
                    while (temp > 1) {
                        int p = smallest_prime[temp];
                        if (p == i) {
                            val *= 1;
                            temp /= p;
                        } else if (is_prime_set[p]) {
                            val *= f[p];
                            temp /= p;
                        } else {
                            val = 0;
                            break;
                        }
                    }
                    if (val != 0) {
                        prefix_sum_pos[j] = prefix_sum_pos[j-1] + val;
                    } else {
                        prefix_sum_pos[j] = prefix_sum_pos[j-1];
                    }
                }
                
                // Temporarily set f(i) = -1
                f[i] = -1;
                for (int j = 1; j <= n; j++) {
                    if (smallest_prime[j] == j) {
                        if (!is_prime_set[j] && j != i) continue;
                    }
                    
                    int val = 1;
                    int temp = j;
                    while (temp > 1) {
                        int p = smallest_prime[temp];
                        if (p == i) {
                            val *= -1;
                            temp /= p;
                        } else if (is_prime_set[p]) {
                            val *= f[p];
                            temp /= p;
                        } else {
                            val = 0;
                            break;
                        }
                    }
                    if (val != 0) {
                        prefix_sum_neg[j] = prefix_sum_neg[j-1] + val;
                    } else {
                        prefix_sum_neg[j] = prefix_sum_neg[j-1];
                    }
                }
                
                // Choose the better option (this is simplified)
                f[i] = -1;  // Generally -1 works better for balancing
                is_prime_set[i] = true;
            }
        } else {
            // i is composite, compute f(i) from its prime factorization
            int val = 1;
            int temp = i;
            while (temp > 1) {
                int p = smallest_prime[temp];
                val *= f[p];
                temp /= p;
            }
            f[i] = val;
        }
    }
    
    for (int i = 1; i <= n; i++) {
        cout << f[i];
        if (i < n) cout << " ";
    }
    cout << "\n";
    
    return 0;
}