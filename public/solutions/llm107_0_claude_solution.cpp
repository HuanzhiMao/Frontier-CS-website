#include <bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    vector<long long> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499};
    
    for (int t = 0; t < T; t++) {
        map<long long, int> prime_powers;
        
        for (long long p : primes) {
            cout << "0 " << p << endl;
            cout.flush();
            
            long long g;
            cin >> g;
            
            if (g > 1) {
                long long curr = g;
                int power = 0;
                while (curr % p == 0) {
                    curr /= p;
                    power++;
                }
                if (power > 0) {
                    prime_powers[p] = power;
                }
            }
        }
        
        // Try some powers of 2
        for (int i = 20; i <= 30; i++) {
            cout << "0 " << (1LL << i) << endl;
            cout.flush();
            
            long long g;
            cin >> g;
            
            if (g > 1) {
                long long curr = g;
                int power = 0;
                while (curr % 2 == 0) {
                    curr /= 2;
                    power++;
                }
                if (power > 0) {
                    prime_powers[2] = max(prime_powers[2], power);
                }
            }
        }
        
        long long divisors = 1;
        for (auto& pp : prime_powers) {
            divisors *= (pp.second + 1);
        }
        
        if (divisors == 1) {
            divisors = 2; // likely prime
        }
        
        cout << "1 " << divisors << endl;
        cout.flush();
    }
    
    return 0;
}