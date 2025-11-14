#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> p(n + 1);
    vector<vector<int>> orValues(n + 1, vector<int>(n + 1, -1));
    
    // Query all pairs to get OR values
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            cout << "? " << i << " " << j << "\n";
            cout.flush();
            cin >> orValues[i][j];
            orValues[j][i] = orValues[i][j];
            if (orValues[i][j] == -1) return 0;
        }
    }
    
    // Try to determine the permutation
    vector<bool> used(n);
    vector<bool> assigned(n + 1, false);
    
    // Start with position 1
    for (int val1 = 0; val1 < n; val1++) {
        if (used[val1]) continue;
        
        vector<int> tempP(n + 1);
        vector<bool> tempUsed(n);
        vector<bool> tempAssigned(n + 1, false);
        
        tempP[1] = val1;
        tempUsed[val1] = true;
        tempAssigned[1] = true;
        
        bool valid = true;
        
        // Try to determine other positions
        for (int pos = 2; pos <= n && valid; pos++) {
            bool found = false;
            
            for (int val = 0; val < n && !found; val++) {
                if (tempUsed[val]) continue;
                
                bool canAssign = true;
                
                // Check consistency with all previously assigned positions
                for (int prevPos = 1; prevPos < pos; prevPos++) {
                    int expectedOr = tempP[prevPos] | val;
                    if (orValues[prevPos][pos] != expectedOr) {
                        canAssign = false;
                        break;
                    }
                }
                
                if (canAssign) {
                    tempP[pos] = val;
                    tempUsed[val] = true;
                    tempAssigned[pos] = true;
                    found = true;
                }
            }
            
            if (!found) {
                valid = false;
            }
        }
        
        if (valid) {
            // Verify the complete solution
            bool allValid = true;
            for (int i = 1; i <= n && allValid; i++) {
                for (int j = i + 1; j <= n && allValid; j++) {
                    if ((tempP[i] | tempP[j]) != orValues[i][j]) {
                        allValid = false;
                    }
                }
            }
            
            if (allValid) {
                cout << "!";
                for (int i = 1; i <= n; i++) {
                    cout << " " << tempP[i];
                }
                cout << "\n";
                cout.flush();
                return 0;
            }
        }
    }
    
    return 0;
}