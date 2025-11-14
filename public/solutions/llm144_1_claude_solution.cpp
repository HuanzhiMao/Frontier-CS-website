#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    // First query with all indices to get the global medians
    cout << "0 " << n;
    for (int i = 1; i <= n; i++) {
        cout << " " << i;
    }
    cout << endl;
    cout.flush();
    
    int global_m1, global_m2;
    cin >> global_m1 >> global_m2;
    
    // Now we need to find which indices contain these median values
    vector<int> candidates;
    for (int i = 1; i <= n; i++) {
        candidates.push_back(i);
    }
    
    // Try to isolate the positions of global_m1 and global_m2
    int pos_m1 = -1, pos_m2 = -1;
    
    // For each position, test if removing it changes the medians
    for (int i = 1; i <= n; i++) {
        if (pos_m1 != -1 && pos_m2 != -1) break;
        
        vector<int> test_indices;
        for (int j = 1; j <= n; j++) {
            if (j != i) {
                test_indices.push_back(j);
            }
        }
        
        // We need even length, so if we have odd length, remove one more
        if (test_indices.size() % 2 == 1) {
            test_indices.pop_back();
        }
        
        if (test_indices.size() >= 4) {
            cout << "0 " << test_indices.size();
            for (int idx : test_indices) {
                cout << " " << idx;
            }
            cout << endl;
            cout.flush();
            
            int m1, m2;
            cin >> m1 >> m2;
            
            // If removing position i changes the medians, then i likely contains one of the global medians
            if (m1 != global_m1 || m2 != global_m2) {
                if (pos_m1 == -1) {
                    pos_m1 = i;
                } else if (pos_m2 == -1 && i != pos_m1) {
                    pos_m2 = i;
                }
            }
        }
    }
    
    // If we haven't found both positions, try a different approach
    if (pos_m1 == -1 || pos_m2 == -1) {
        // Try smaller subsets to isolate the medians
        for (int i = 1; i <= n && (pos_m1 == -1 || pos_m2 == -1); i++) {
            for (int j = i + 1; j <= n && (pos_m1 == -1 || pos_m2 == -1); j++) {
                vector<int> remaining;
                for (int k = 1; k <= n; k++) {
                    if (k != i && k != j) {
                        remaining.push_back(k);
                    }
                }
                
                if (remaining.size() >= 4 && remaining.size() % 2 == 0) {
                    cout << "0 " << remaining.size();
                    for (int idx : remaining) {
                        cout << " " << idx;
                    }
                    cout << endl;
                    cout.flush();
                    
                    int m1, m2;
                    cin >> m1 >> m2;
                    
                    if ((m1 == global_m1 && m2 == global_m2) || (m1 == global_m2 && m2 == global_m1)) {
                        continue;
                    } else {
                        // The removed positions likely contain the medians
                        pos_m1 = i;
                        pos_m2 = j;
                        break;
                    }
                }
            }
        }
    }
    
    // Make sure we have valid positions
    if (pos_m1 == -1 || pos_m2 == -1) {
        // Fallback: just guess based on the pattern
        pos_m1 = n/2;
        pos_m2 = n/2 + 1;
    }
    
    // Ensure pos_m1 < pos_m2
    if (pos_m1 > pos_m2) {
        swap(pos_m1, pos_m2);
    }
    
    cout << "1 " << pos_m1 << " " << pos_m2 << endl;
    cout.flush();
    
    return 0;
}