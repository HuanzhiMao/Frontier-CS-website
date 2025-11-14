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
            int count = 0;
            
            while (power <= 1e18 / p) {
                cout << "0 " << power << endl;
                cout.flush();
                
                long long g;
                cin >> g;
                
                if (g == power) {
                    count++;
                    power *= p;
                } else {
                    break;
                }
            }
            
            if (count > 0) {
                prime_powers[p] = count;
            }
        }
        
        // Check some larger primes
        vector<long long> large_primes;
        for (int i = 0; i < 10 && i < primes.size(); i++) {
            long long candidate = 1000000007 + i * 2;
            cout << "0 " << candidate << endl;
            cout.flush();
            
            long long g;
            cin >> g;
            
            if (g > 1 && g != candidate) {
                // Found a factor
                bool is_new = true;
                for (auto& pp : prime_powers) {
                    if (g % pp.first == 0) {
                        is_new = false;
                        break;
                    }
                }
                if (is_new) {
                    prime_powers[g] = 1;
                }
            }
        }
        
        // Calculate number of divisors
        long long divisors = 1;
        for (auto& pp : prime_powers) {
            divisors *= (pp.second + 1);
        }
        
        if (divisors == 1) {
            // Likely a large prime or we missed factors
            divisors = 2;
        }
        
        cout << "1 " << divisors << endl;
        cout.flush();
    }
    
    return 0;
}