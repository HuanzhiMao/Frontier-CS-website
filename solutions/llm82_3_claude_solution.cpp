#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> p(n + 1);
    vector<vector<int>> or_result(n + 1, vector<int>(n + 1, -1));
    
    // Query all pairs
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            cout << "? " << i << " " << j << endl;
            cout.flush();
            int result;
            cin >> result;
            if (result == -1) return 0;
            or_result[i][j] = or_result[j][i] = result;
        }
    }
    
    // Find the maximum bit position needed
    int max_bit = 0;
    while ((1 << max_bit) < n) max_bit++;
    
    // For each bit position, determine which elements have that bit set
    for (int bit = 0; bit < max_bit; bit++) {
        vector<bool> has_bit(n + 1, false);
        vector<int> candidates;
        
        // Find candidates that might have this bit set
        for (int i = 1; i <= n; i++) {
            bool might_have = false;
            for (int j = 1; j <= n; j++) {
                if (i != j && (or_result[i][j] & (1 << bit))) {
                    might_have = true;
                    break;
                }
            }
            if (might_have) {
                candidates.push_back(i);
            }
        }
        
        // Use constraint satisfaction to determine which actually have the bit
        for (int mask = 0; mask < (1 << candidates.size()); mask++) {
            vector<bool> test_has_bit(n + 1, false);
            for (int i = 0; i < candidates.size(); i++) {
                if (mask & (1 << i)) {
                    test_has_bit[candidates[i]] = true;
                }
            }
            
            bool valid = true;
            for (int i = 1; i <= n && valid; i++) {
                for (int j = i + 1; j <= n && valid; j++) {
                    bool should_have_bit = (or_result[i][j] & (1 << bit)) != 0;
                    bool would_have_bit = test_has_bit[i] || test_has_bit[j];
                    if (should_have_bit != would_have_bit) {
                        valid = false;
                    }
                }
            }
            
            if (valid) {
                has_bit = test_has_bit;
                break;
            }
        }
        
        // Set the bit for elements that have it
        for (int i = 1; i <= n; i++) {
            if (has_bit[i]) {
                p[i] |= (1 << bit);
            }
        }
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << p[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}