#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long n;
    cin >> n;
    
    vector<long long> result;
    
    // Start with powers of 2 as they give good gcd progression
    long long power = 1;
    while (power <= n) {
        result.push_back(power);
        if (power > n / 2) break;
        power *= 2;
    }
    
    // Try to extend with multiples that maintain gcd progression
    long long last = result.back();
    long long prev_last = result[result.size() - 2];
    long long current_gcd = __gcd(last, prev_last);
    
    // Look for numbers that can extend the sequence
    for (long long mult = 2; mult <= 100 && result.size() < 50; mult++) {
        long long candidate = last * mult;
        if (candidate > n) break;
        
        long long new_gcd = __gcd(candidate, last);
        if (new_gcd > current_gcd) {
            result.push_back(candidate);
            prev_last = last;
            last = candidate;
            current_gcd = new_gcd;
        }
    }
    
    // If we have room, try adding some large numbers that fit the pattern
    if (result.size() < 20) {
        long long target_gcd = current_gcd * 2;
        for (long long i = max(last + 1, n - 1000); i <= n && result.size() < 30; i++) {
            if (i > last && __gcd(i, last) > current_gcd) {
                result.push_back(i);
                current_gcd = __gcd(i, last);
                last = i;
                break;
            }
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