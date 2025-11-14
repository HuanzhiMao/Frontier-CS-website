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
            
            long long largest_power = 1LL << (bits - 1);
            
            if (largest_power == remaining) {
                // If remaining is exactly a power of 2, we can use all ascending
                for (int i = 0; i < bits - 1; i++) {
                    result.push_back(i);
                }
                break;
            } else {
                // Use descending elements to subtract powers of 2
                long long need_to_subtract = remaining - largest_power;
                
                // Find how many descending elements we need
                int desc_count = 0;
                long long power = 1;
                while (need_to_subtract > 0) {
                    if (need_to_subtract & 1) {
                        result.push_back(result.size() + desc_count);
                    }
                    need_to_subtract >>= 1;
                    desc_count++;
                }
                
                // Add one ascending element
                result.push_back(result.size() - desc_count);
                remaining = largest_power;
            }
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