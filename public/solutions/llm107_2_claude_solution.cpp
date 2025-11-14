#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        map<long long, int> prime_powers;
        vector<long long> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199};
        
        // Check small primes and their powers
        for (long long p : primes) {
            long long power = p;
            int exp = 1;
            
            while (power <= 1e9) {
                cout << "0 " << power << endl;
                cout.flush();
                long long g;
                cin >> g;
                
                if (g == power) {
                    prime_powers[p] = exp;
                } else if (g > 1) {
                    // Found a factor
                    long long curr = g;
                    int cnt = 0;
                    while (curr % p == 0) {
                        curr /= p;
                        cnt++;
                    }
                    prime_powers[p] = max(prime_powers[p], cnt);
                }
                
                if (power > 1e9 / p) break;
                power *= p;
                exp++;
            }
        }
        
        // Calculate divisor count
        long long divisors = 1;
        for (auto& [p, exp] : prime_powers) {
            if (exp > 0) {
                divisors *= (exp + 1);
            }
        }
        
        if (divisors == 1) {
            // Might be a large prime or composite with large prime factors
            // Try some random queries to see if we get any non-1 gcd
            vector<long long> randoms = {1000000007, 1000000009, 998244353, 1e9 + 21, 1e9 + 33};
            bool found_factor = false;
            
            for (long long r : randoms) {
                cout << "0 " << r << endl;
                cout.flush();
                long long g;
                cin >> g;
                
                if (g > 1) {
                    found_factor = true;
                    break;
                }
            }
            
            if (!found_factor) {
                divisors = 2; // Likely a large prime
            } else {
                divisors = 4; // Has some factors
            }
        }
        
        cout << "1 " << divisors << endl;
        cout.flush();
    }
    
    return 0;
}