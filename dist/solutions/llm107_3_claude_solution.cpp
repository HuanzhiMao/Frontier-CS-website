#include <bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        map<long long, int> prime_powers;
        vector<long long> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199};
        
        // Query powers of small primes
        for (long long p : primes) {
            long long power = p;
            int max_power = 0;
            
            while (power <= 1e18 / p) {
                cout << "0 " << power << endl;
                cout.flush();
                
                long long g;
                cin >> g;
                
                if (g == 1) break;
                
                // Count how many times p divides g
                long long temp = g;
                int cnt = 0;
                while (temp % p == 0) {
                    temp /= p;
                    cnt++;
                }
                max_power = max(max_power, cnt);
                
                power *= p;
            }
            
            if (max_power > 0) {
                prime_powers[p] = max_power;
            }
        }
        
        // Calculate divisors count
        long long divisors = 1;
        for (auto& [prime, power] : prime_powers) {
            divisors *= (power + 1);
        }
        
        // If no small prime factors found, likely prime or has large prime factors
        if (divisors == 1) {
            // Try a few random large numbers to see if we get gcd > 1
            bool found_factor = false;
            for (int i = 0; i < 5; i++) {
                long long q = 1000000007LL + i * 982451653LL;
                cout << "0 " << q << endl;
                cout.flush();
                
                long long g;
                cin >> g;
                
                if (g > 1) {
                    found_factor = true;
                    break;
                }
            }
            
            if (!found_factor) {
                divisors = 2; // Likely prime
            } else {
                divisors = 4; // Has some factors
            }
        }
        
        cout << "1 " << divisors << endl;
        cout.flush();
    }
    
    return 0;
}