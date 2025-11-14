#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int q;
    cin >> q;
    
    while (q--) {
        long long k;
        cin >> k;
        
        vector<int> result;
        long long remaining = k;
        
        while (remaining > 1) {
            // Find the largest power of 2 that is <= remaining
            int bits = 0;
            long long temp = remaining;
            while (temp > 0) {
                bits++;
                temp /= 2;
            }
            
            long long power = 1LL << (bits - 1);
            
            if (power == remaining) {
                // If remaining is exactly a power of 2, we need bits-1 ascending elements
                for (int i = 0; i < bits - 1; i++) {
                    result.push_back(i);
                }
                break;
            } else {
                // Add one descending element
                result.push_back(result.size());
                remaining -= power;
            }
        }
        
        // Reverse to get the correct permutation
        reverse(result.begin(), result.end());
        for (int i = 0; i < result.size(); i++) {
            result[i] = result.size() - 1 - result[i];
        }
        
        cout << result.size() << "\n";
        for (int i = 0; i < result.size(); i++) {
            cout << result[i];
            if (i < result.size() - 1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}