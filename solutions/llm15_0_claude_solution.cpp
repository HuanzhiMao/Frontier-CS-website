#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    vector<int> p(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i];
    }
    
    vector<pair<int, int>> operations;
    
    for (int target = 1; target <= n; target++) {
        // Find current position of target
        int pos = 0;
        for (int i = 0; i < n; i++) {
            if (p[i] == target) {
                pos = i;
                break;
            }
        }
        
        // If already in correct position, continue
        if (pos == target - 1) continue;
        
        // Try to move target to position target-1
        bool moved = false;
        
        // Try all possible operations
        for (int x = 1; x < n && !moved; x++) {
            for (int y = 1; x + y < n && !moved; y++) {
                int mid_start = x;
                int mid_end = n - y - 1;
                
                // Check if this operation would move target to correct position
                int new_pos = -1;
                if (pos < x) { // target in prefix
                    new_pos = n - y + pos;
                } else if (pos >= n - y) { // target in suffix
                    new_pos = pos - (n - y);
                } else { // target in middle
                    new_pos = pos;
                }
                
                if (new_pos == target - 1) {
                    // Perform the operation
                    vector<int> new_p(n);
                    // Copy suffix to beginning
                    for (int i = 0; i < y; i++) {
                        new_p[i] = p[n - y + i];
                    }
                    // Copy middle
                    for (int i = 0; i < n - x - y; i++) {
                        new_p[y + i] = p[x + i];
                    }
                    // Copy prefix to end
                    for (int i = 0; i < x; i++) {
                        new_p[n - x + i] = p[i];
                    }
                    
                    p = new_p;
                    operations.push_back({x, y});
                    moved = true;
                }
            }
        }
        
        // If we couldn't move it directly, try a different approach
        if (!moved && pos != target - 1) {
            // Try to move it closer or to a better position for next move
            for (int x = 1; x < n && !moved; x++) {
                for (int y = 1; x + y < n && !moved; y++) {
                    int new_pos = -1;
                    if (pos < x) { // target in prefix
                        new_pos = n - y + pos;
                    } else if (pos >= n - y) { // target in suffix
                        new_pos = pos - (n - y);
                    } else { // target in middle
                        new_pos = pos;
                    }
                    
                    if (new_pos < pos) { // If it moves closer to front
                        vector<int> new_p(n);
                        // Copy suffix to beginning
                        for (int i = 0; i < y; i++) {
                            new_p[i] = p[n - y + i];
                        }
                        // Copy middle
                        for (int i = 0; i < n - x - y; i++) {
                            new_p[y + i] = p[x + i];
                        }
                        // Copy prefix to end
                        for (int i = 0; i < x; i++) {
                            new_p[n - x + i] = p[i];
                        }
                        
                        p = new_p;
                        operations.push_back({x, y});
                        moved = true;
                        target--; // Retry this target
                    }
                }
            }
        }
    }
    
    cout << operations.size() << "\n";
    for (auto op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}