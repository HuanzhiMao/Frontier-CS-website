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
        k--; // Subtract 1 for empty subsequence
        
        int pos = 0;
        while (k > 0) {
            // Find largest power of 2 <= k
            int bits = 0;
            long long temp = k;
            while (temp > 0) {
                bits++;
                temp /= 2;
            }
            
            long long power = 1LL << (bits - 1);
            if (power > k) {
                power = 1LL << (bits - 2);
                bits--;
            }
            
            // Add 'bits' elements in decreasing order
            for (int i = pos + bits - 1; i >= pos; i--) {
                result.push_back(i);
            }
            
            pos += bits;
            k -= power;
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