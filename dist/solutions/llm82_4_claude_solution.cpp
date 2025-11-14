#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> p(n + 1);
    vector<vector<int>> orValues(n + 1, vector<int>(n + 1, -1));
    
    // Query all pairs
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            cout << "? " << i << " " << j << "\n";
            cout.flush();
            int result;
            cin >> result;
            if (result == -1) return 0;
            orValues[i][j] = orValues[j][i] = result;
        }
    }
    
    // Find the element that is 0
    int zeroPos = -1;
    for (int i = 1; i <= n; i++) {
        bool isZero = true;
        for (int j = 1; j <= n; j++) {
            if (i == j) continue;
            // Check if orValues[i][j] could be consistent with p[i] = 0
            // This means orValues[i][j] should equal p[j]
            bool consistent = true;
            for (int k = 1; k <= n; k++) {
                if (k == i || k == j) continue;
                // orValues[j][k] should equal p[j] | p[k]
                // If p[i] = 0, then orValues[i][j] = p[j]
                // So we need orValues[j][k] to be consistent with orValues[i][j] | p[k]
            }
        }
        
        // Try a different approach - if p[i] = 0, then orValues[i][j] = p[j] for all j
        vector<int> candidates;
        for (int j = 1; j <= n; j++) {
            if (i != j) {
                candidates.push_back(orValues[i][j]);
            }
        }
        
        // Check if these candidates form a valid set when one position is 0
        sort(candidates.begin(), candidates.end());
        bool valid = true;
        
        // Verify consistency
        for (int j = 1; j <= n && valid; j++) {
            if (i == j) continue;
            for (int k = j + 1; k <= n && valid; k++) {
                if (i == k) continue;
                int expectedOr = orValues[i][j] | orValues[i][k];
                if (orValues[j][k] != expectedOr) {
                    valid = false;
                }
            }
        }
        
        if (valid) {
            zeroPos = i;
            break;
        }
    }
    
    // If we found the zero position
    if (zeroPos != -1) {
        p[zeroPos] = 0;
        // Fill in other positions
        for (int j = 1; j <= n; j++) {
            if (j != zeroPos) {
                p[j] = orValues[zeroPos][j];
            }
        }
    } else {
        // Fallback: try to solve by finding patterns
        // Use the fact that for small values, OR operations have specific patterns
        vector<bool> used(n, false);
        
        // Find position with minimum maximum OR value
        int minMaxPos = 1;
        int minMaxVal = INT_MAX;
        for (int i = 1; i <= n; i++) {
            int maxOr = 0;
            for (int j = 1; j <= n; j++) {
                if (i != j) {
                    maxOr = max(maxOr, orValues[i][j]);
                }
            }
            if (maxOr < minMaxVal) {
                minMaxVal = maxOr;
                minMaxPos = i;
            }
        }
        
        p[minMaxPos] = 0;
        used[0] = true;
        
        for (int j = 1; j <= n; j++) {
            if (j != minMaxPos) {
                p[j] = orValues[minMaxPos][j];
                if (p[j] < n) used[p[j]] = true;
            }
        }
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << p[i];
    }
    cout << "\n";
    cout.flush();
    
    return 0;
}