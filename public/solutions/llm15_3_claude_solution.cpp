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
    
    // Try to sort the array by moving elements to their correct positions
    for (int target = 1; target <= n && operations.size() < 4 * n; target++) {
        // Find where target currently is
        int pos = -1;
        for (int i = 0; i < n; i++) {
            if (p[i] == target) {
                pos = i;
                break;
            }
        }
        
        // If target is already in correct position, continue
        if (pos == target - 1) continue;
        
        // Try to move target to position target-1
        int target_pos = target - 1;
        
        // We need to find x and y such that after the operation,
        // element at position pos goes to position target_pos
        
        // Try different values of x and y
        bool found = false;
        for (int x = 1; x < n && !found && operations.size() < 4 * n; x++) {
            for (int y = 1; x + y < n && !found && operations.size() < 4 * n; y++) {
                vector<int> temp = p;
                
                // Perform the operation on temp array
                vector<int> prefix(temp.begin(), temp.begin() + x);
                vector<int> middle(temp.begin() + x, temp.begin() + n - y);
                vector<int> suffix(temp.begin() + n - y, temp.end());
                
                // Reconstruct: [Suffix | Middle | Prefix]
                temp.clear();
                temp.insert(temp.end(), suffix.begin(), suffix.end());
                temp.insert(temp.end(), middle.begin(), middle.end());
                temp.insert(temp.end(), prefix.begin(), prefix.end());
                
                // Check if this operation brings us closer to sorted
                bool better = true;
                for (int i = 0; i < target; i++) {
                    if (temp[i] != i + 1) {
                        better = false;
                        break;
                    }
                }
                
                if (better && temp[target - 1] == target) {
                    p = temp;
                    operations.push_back({x, y});
                    found = true;
                }
            }
        }
        
        // If we couldn't find a direct move, try a greedy approach
        if (!found && operations.size() < 4 * n - 1) {
            // Find any operation that improves the lexicographic order
            vector<int> best = p;
            int best_x = -1, best_y = -1;
            
            for (int x = 1; x < n && operations.size() < 4 * n; x++) {
                for (int y = 1; x + y < n && operations.size() < 4 * n; y++) {
                    vector<int> temp = p;
                    
                    // Perform the operation
                    vector<int> prefix(temp.begin(), temp.begin() + x);
                    vector<int> middle(temp.begin() + x, temp.begin() + n - y);
                    vector<int> suffix(temp.begin() + n - y, temp.end());
                    
                    temp.clear();
                    temp.insert(temp.end(), suffix.begin(), suffix.end());
                    temp.insert(temp.end(), middle.begin(), middle.end());
                    temp.insert(temp.end(), prefix.begin(), prefix.end());
                    
                    if (temp < best) {
                        best = temp;
                        best_x = x;
                        best_y = y;
                    }
                }
            }
            
            if (best_x != -1) {
                p = best;
                operations.push_back({best_x, best_y});
            }
        }
    }
    
    cout << operations.size() << "\n";
    for (auto op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}