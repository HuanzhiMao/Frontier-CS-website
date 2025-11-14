#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        map<long long, int> prime_powers;
        vector<long long> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
        
        // Query small primes and their powers
        for (long long p : primes) {
            cout << "0 " << p << endl;
            cout.flush();
            long long g;
            cin >> g;
            
            if (g > 1) {
                // Find the exact power of p in X
                long long power = p;
                int count = 1;
                
                while (power <= 1e9) {
                    cout << "0 " << power * p << endl;
                    cout.flush();
                    long long g2;
                    cin >> g2;
                    
                    if (g2 == power * p) {
                        power *= p;
                        count++;
                    } else {
                        break;
                    }
                }
                
                prime_powers[p] = count;
            }
        }
        
        // Calculate number of divisors
        long long divisors = 1;
        for (auto& pp : prime_powers) {
            divisors *= (pp.second + 1);
        }
        
        // If no small prime factors found, likely prime or has large prime factors
        if (prime_powers.empty()) {
            // Try some random queries to see if we can find any factor
            vector<long long> randoms = {1000000007, 1000000009, 998244353, 1073741827};
            bool found_factor = false;
            
            for (long long r : randoms) {
                cout << "0 " << r << endl;
                cout.flush();
                long long g;
                cin >> g;
                
                if (g > 1) {
                    found_factor = true;
                    divisors = 4; // rough estimate
                    break;
                }
            }
            
            if (!found_factor) {
                divisors = 2; // likely prime
            }
        }
        
        cout << "1 " << divisors << endl;
        cout.flush();
    }
    
    return 0;
}