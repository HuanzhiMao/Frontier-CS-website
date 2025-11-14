#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long n;
    cin >> n;
    
    vector<long long> sequence;
    
    // Start with 1
    sequence.push_back(1);
    
    // Try to build a sequence with powers of 2, but use larger numbers when possible
    long long current = 1;
    long long prev_gcd = 1;
    
    while (true) {
        long long next_candidate = current * 2;
        if (next_candidate > n) break;
        
        long long next_gcd = __gcd(next_candidate, current);
        if (sequence.size() == 1 || next_gcd > prev_gcd) {
            sequence.push_back(next_candidate);
            prev_gcd = next_gcd;
            current = next_candidate;
        } else {
            break;
        }
    }
    
    // Try to extend with multiples that maintain the gcd condition
    bool extended = true;
    while (extended) {
        extended = false;
        long long best_candidate = -1;
        long long best_gcd = prev_gcd;
        
        // Try multiples of current number
        for (long long mult = 2; mult <= min(100LL, n / current); mult++) {
            long long candidate = current * mult;
            if (candidate > n) break;
            
            long long candidate_gcd = __gcd(candidate, current);
            if (candidate_gcd > prev_gcd && candidate_gcd > best_gcd) {
                best_candidate = candidate;
                best_gcd = candidate_gcd;
            }
        }
        
        if (best_candidate != -1) {
            sequence.push_back(best_candidate);
            prev_gcd = best_gcd;
            current = best_candidate;
            extended = true;
        }
    }
    
    // If sequence is short, try a different approach with larger starting numbers
    if (sequence.size() < 10 && n >= 1000) {
        vector<long long> alt_sequence;
        
        // Start with a larger base
        long long base = min(n / 100, 1000000LL);
        alt_sequence.push_back(base);
        
        long long alt_current = base;
        long long alt_prev_gcd = 1;
        
        for (int i = 2; i <= 6 && alt_current * i <= n; i++) {
            long long candidate = alt_current * i;
            long long candidate_gcd = __gcd(candidate, alt_current);
            
            if (alt_sequence.size() == 1 || candidate_gcd > alt_prev_gcd) {
                alt_sequence.push_back(candidate);
                alt_prev_gcd = candidate_gcd;
                alt_current = candidate;
            }
        }
        
        // Calculate objectives
        long long obj1 = sequence.size();
        for (long long x : sequence) obj1 += x;
        obj1 *= sequence.size();
        
        long long obj2 = alt_sequence.size();
        for (long long x : alt_sequence) obj2 += x;
        obj2 *= alt_sequence.size();
        
        if (obj2 > obj1) {
            sequence = alt_sequence;
        }
    }
    
    cout << sequence.size() << "\n";
    for (int i = 0; i < sequence.size(); i++) {
        if (i > 0) cout << " ";
        cout << sequence[i];
    }
    cout << "\n";
    
    return 0;
}