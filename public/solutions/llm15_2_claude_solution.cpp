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
    
    // Greedy approach: try to fix positions from left to right
    for (int target = 1; target <= n && operations.size() < 4 * n; target++) {
        // Find current position of target
        int pos = -1;
        for (int i = 0; i < n; i++) {
            if (p[i] == target) {
                pos = i;
                break;
            }
        }
        
        // If target is already in correct position, continue
        if (pos == target - 1) continue;
        
        // Try to move target to correct position
        bool moved = false;
        
        // Try different operations to bring target closer to front
        for (int x = 1; x < n - 1 && !moved && operations.size() < 4 * n; x++) {
            for (int y = 1; x + y < n && !moved && operations.size() < 4 * n; y++) {
                vector<int> temp = p;
                
                // Apply operation: swap prefix of length x with suffix of length y
                vector<int> prefix(temp.begin(), temp.begin() + x);
                vector<int> middle(temp.begin() + x, temp.begin() + n - y);
                vector<int> suffix(temp.begin() + n - y, temp.end());
                
                temp.clear();
                temp.insert(temp.end(), suffix.begin(), suffix.end());
                temp.insert(temp.end(), middle.begin(), middle.end());
                temp.insert(temp.end(), prefix.begin(), prefix.end());
                
                // Check if this operation makes the permutation lexicographically smaller
                if (temp < p) {
                    p = temp;
                    operations.push_back({x, y});
                    moved = true;
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