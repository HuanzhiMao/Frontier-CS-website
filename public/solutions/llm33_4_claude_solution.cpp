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
            // Find largest power of 2 <= remaining
            long long power = 1;
            int count = 0;
            while (power * 2 <= remaining) {
                power *= 2;
                count++;
            }
            
            // Add 'count' elements in decreasing order
            for (int i = 0; i < count; i++) {
                result.push_back(next_val + count - 1 - i);
            }
            next_val += count;
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