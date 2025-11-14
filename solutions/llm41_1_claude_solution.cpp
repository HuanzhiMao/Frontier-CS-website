#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long n;
    cin >> n;
    
    vector<long long> result;
    
    // Start with 1
    result.push_back(1);
    
    // Add powers of 2 up to n
    long long power = 2;
    while (power <= n) {
        result.push_back(power);
        if (power > n / 2) break;
        power *= 2;
    }
    
    // Try to extend with multiples that maintain increasing GCD
    long long last = result.back();
    long long last_gcd = last / 2; // gcd of last two elements
    
    // Look for next number that maintains the property
    for (long long mult = 3; mult <= min(n / last, 1000LL); mult++) {
        long long candidate = last * mult;
        if (candidate > n) break;
        
        long long new_gcd = __gcd(candidate, last);
        if (new_gcd > last_gcd) {
            result.push_back(candidate);
            last_gcd = new_gcd;
            last = candidate;
        }
    }
    
    // If we can extend further with larger jumps
    while (result.size() < 50) {
        bool found = false;
        long long best_candidate = 0;
        long long best_gcd = last_gcd;
        
        // Try multiples of the last element
        for (long long mult = 2; mult <= min(n / last, 100LL); mult++) {
            long long candidate = last * mult;
            if (candidate > n) break;
            
            long long new_gcd = __gcd(candidate, last);
            if (new_gcd > last_gcd && new_gcd > best_gcd) {
                best_candidate = candidate;
                best_gcd = new_gcd;
                found = true;
            }
        }
        
        if (found) {
            result.push_back(best_candidate);
            last_gcd = best_gcd;
            last = best_candidate;
        } else {
            break;
        }
    }
    
    cout << result.size() << "\n";
    for (int i = 0; i < result.size(); i++) {
        cout << result[i];
        if (i < result.size() - 1) cout << " ";
    }
    cout << "\n";
    
    return 0;
}