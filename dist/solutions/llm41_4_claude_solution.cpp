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
    
    // Add powers of 2 that are <= n
    long long power = 2;
    while (power <= n) {
        result.push_back(power);
        if (power > n / 2) break;
        power *= 2;
    }
    
    // Try to extend with multiples that maintain the gcd property
    long long last = result.back();
    long long last_gcd = last / 2; // gcd of last two elements
    
    // Look for next number that satisfies conditions
    for (long long mult = 3; mult <= min(n / last, (long long)1000); mult++) {
        long long candidate = last * mult;
        if (candidate <= n) {
            long long new_gcd = __gcd(candidate, last);
            if (new_gcd > last_gcd) {
                result.push_back(candidate);
                last = candidate;
                last_gcd = new_gcd;
                break;
            }
        }
    }
    
    cout << result.size() << "\n";
    for (int i = 0; i < result.size(); i++) {
        if (i > 0) cout << " ";
        cout << result[i];
    }
    cout << "\n";
    
    return 0;
}