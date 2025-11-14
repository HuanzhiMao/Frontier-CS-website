#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> p(n + 1);
    
    // For each position i from 1 to n, find p[i]
    for (int i = 1; i <= n; i++) {
        // Find how many elements to the left of position i are greater than p[i]
        int inversions = 0;
        
        if (i > 1) {
            // Query inversions in [1, i]
            cout << "0 1 " << i << endl;
            cout.flush();
            int total_inv;
            cin >> total_inv;
            
            // Query inversions in [1, i-1]
            cout << "0 1 " << (i-1) << endl;
            cout.flush();
            int prev_inv;
            cin >> prev_inv;
            
            // The difference gives us inversions involving position i
            inversions = (total_inv - prev_inv + 2) % 2;
        }
        
        // Now we know the parity of inversions involving position i
        // We need to determine p[i] based on this information
        
        // Create list of available values
        vector<bool> used(n + 1, false);
        for (int j = 1; j < i; j++) {
            used[p[j]] = true;
        }
        
        vector<int> available;
        for (int j = 1; j <= n; j++) {
            if (!used[j]) {
                available.push_back(j);
            }
        }
        
        // Count how many elements to the left are greater than each candidate
        for (int candidate : available) {
            int count = 0;
            for (int j = 1; j < i; j++) {
                if (p[j] > candidate) {
                    count++;
                }
            }
            
            if (count % 2 == inversions) {
                p[i] = candidate;
                break;
            }
        }
    }
    
    cout << "1";
    for (int i = 1; i <= n; i++) {
        cout << " " << p[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}