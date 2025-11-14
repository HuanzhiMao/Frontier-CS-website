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
        int next_val = 0;
        
        while (remaining > 1) {
            // Find the largest power of 2 that is <= remaining
            long long power = 1;
            int bits = 0;
            while (power * 2 <= remaining) {
                power *= 2;
                bits++;
            }
            
            // Add 'bits' decreasing elements, then one increasing element
            for (int i = bits; i >= 1; i--) {
                result.push_back(next_val + i);
            }
            result.push_back(next_val);
            
            next_val += bits + 1;
            remaining -= power;
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