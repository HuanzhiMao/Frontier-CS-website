#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> p(n + 1);
    vector<bool> used(n + 1, false);
    
    // For each position from 1 to n
    for (int pos = 1; pos <= n; pos++) {
        // Try each possible value that hasn't been used
        for (int val = 1; val <= n; val++) {
            if (used[val]) continue;
            
            // Temporarily assign this value
            p[pos] = val;
            
            // Check if this assignment is consistent with all previous positions
            bool consistent = true;
            
            // Check consistency with queries involving this position and previous positions
            for (int i = 1; i < pos && consistent; i++) {
                // Query the inversion parity for subarray [i, pos]
                cout << "0 " << i << " " << pos << endl;
                cout.flush();
                
                int response;
                cin >> response;
                
                // Calculate expected parity based on current assignment
                int expected_inversions = 0;
                for (int j = i; j < pos; j++) {
                    for (int k = j + 1; k <= pos; k++) {
                        if (p[j] > p[k]) {
                            expected_inversions++;
                        }
                    }
                }
                
                if ((expected_inversions % 2) != response) {
                    consistent = false;
                }
            }
            
            if (consistent) {
                used[val] = true;
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