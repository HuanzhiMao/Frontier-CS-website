#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> p(n + 1);
    vector<vector<int>> inv(n + 1, vector<int>(n + 1, 0));
    
    // Query all pairs to get inversion information
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            cout << "? " << i << " " << j << endl;
            cout.flush();
            cin >> inv[i][j];
        }
    }
    
    // Reconstruct permutation using merge sort approach
    vector<int> pos(n + 1);
    
    // For each position, determine what number should be there
    for (int val = 1; val <= n; val++) {
        // Count how many numbers less than val should come after position of val
        int inversions_after = 0;
        
        // Try each position for val
        for (int pos_val = 1; pos_val <= n; pos_val++) {
            bool valid = true;
            int expected_inv = 0;
            
            // Count inversions involving val at position pos_val
            for (int other_val = 1; other_val <= n; other_val++) {
                if (other_val == val) continue;
                
                bool found_other = false;
                int pos_other = 0;
                
                // Find position of other_val (if already placed)
                for (int i = 1; i <= n; i++) {
                    if (p[i] == other_val) {
                        found_other = true;
                        pos_other = i;
                        break;
                    }
                }
                
                if (found_other) {
                    if (pos_val < pos_other && val > other_val) {
                        expected_inv++;
                    } else if (pos_val > pos_other && val < other_val) {
                        expected_inv++;
                    }
                }
            }
            
            // Check if this matches our queries
            bool matches = true;
            for (int i = 1; i <= n && matches; i++) {
                for (int j = i + 1; j <= n && matches; j++) {
                    int actual_inv = 0;
                    for (int x = i; x < j; x++) {
                        for (int y = x + 1; y <= j; y++) {
                            int val_x = (x == pos_val) ? val : p[x];
                            int val_y = (y == pos_val) ? val : p[y];
                            if (val_x > 0 && val_y > 0 && val_x > val_y) {
                                actual_inv++;
                            }
                        }
                    }
                    if ((actual_inv % 2) != inv[i][j]) {
                        matches = false;
                    }
                }
            }
            
            if (matches && p[pos_val] == 0) {
                p[pos_val] = val;
                break;
            }
        }
    }
    
    cout << "! ";
    for (int i = 1; i <= n; i++) {
        cout << p[i];
        if (i < n) cout << " ";
    }
    cout << endl;
    cout.flush();
    
    return 0;
}