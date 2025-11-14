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
    
    // Greedy approach: try to place smallest elements at the beginning
    for (int target = 1; target <= n && operations.size() < 4 * n; target++) {
        // Find position of target
        int pos = -1;
        for (int i = 0; i < n; i++) {
            if (p[i] == target) {
                pos = i;
                break;
            }
        }
        
        // If target is already in correct position, continue
        if (pos == target - 1) continue;
        
        // Try to move target towards the front
        bool found_move = false;
        
        // Try different operations to bring target closer to front
        for (int x = 1; x < n - 1 && !found_move && operations.size() < 4 * n; x++) {
            for (int y = 1; x + y < n && !found_move && operations.size() < 4 * n; y++) {
                vector<int> temp = p;
                
                // Apply operation: swap prefix of length x with suffix of length y
                vector<int> prefix(temp.begin(), temp.begin() + x);
                vector<int> middle(temp.begin() + x, temp.begin() + n - y);
                vector<int> suffix(temp.begin() + n - y, temp.end());
                
                // Create new permutation: suffix + middle + prefix
                vector<int> new_p;
                new_p.insert(new_p.end(), suffix.begin(), suffix.end());
                new_p.insert(new_p.end(), middle.begin(), middle.end());
                new_p.insert(new_p.end(), prefix.begin(), prefix.end());
                
                // Check if this move improves the permutation lexicographically
                if (new_p < p) {
                    p = new_p;
                    operations.push_back({x, y});
                    found_move = true;
                }
            }
        }
        
        // If no improving move found, try any move that brings target closer
        if (!found_move && operations.size() < 4 * n) {
            for (int x = 1; x < n - 1 && !found_move && operations.size() < 4 * n; x++) {
                for (int y = 1; x + y < n && !found_move && operations.size() < 4 * n; y++) {
                    // Find new position of target after this operation
                    int new_pos = -1;
                    if (pos < x) {
                        new_pos = n - y + pos;
                    } else if (pos >= n - y) {
                        new_pos = pos - (n - y);
                    } else {
                        new_pos = y + (pos - x);
                    }
                    
                    // If target gets closer to its desired position
                    if (new_pos < pos && new_pos >= 0) {
                        vector<int> prefix(p.begin(), p.begin() + x);
                        vector<int> middle(p.begin() + x, p.begin() + n - y);
                        vector<int> suffix(p.begin() + n - y, p.end());
                        
                        p.clear();
                        p.insert(p.end(), suffix.begin(), suffix.end());
                        p.insert(p.end(), middle.begin(), middle.end());
                        p.insert(p.end(), prefix.begin(), prefix.end());
                        
                        operations.push_back({x, y});
                        found_move = true;
                    }
                }
            }
        }
        
        if (!found_move) {
            target--;
        }
    }
    
    cout << operations.size() << "\n";
    for (auto& op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}