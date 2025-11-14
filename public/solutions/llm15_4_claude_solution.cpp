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
    
    // Try to improve the permutation by moving smaller elements to the front
    for (int iter = 0; iter < 4 * n && operations.size() < 4 * n; iter++) {
        bool improved = false;
        vector<int> best_p = p;
        int best_x = -1, best_y = -1;
        
        // Try all possible operations
        for (int x = 1; x <= n - 2 && !improved; x++) {
            for (int y = 1; y <= n - x - 1 && !improved; y++) {
                vector<int> temp = p;
                
                // Apply operation: [Prefix | Middle | Suffix] -> [Suffix | Middle | Prefix]
                vector<int> prefix(temp.begin(), temp.begin() + x);
                vector<int> middle(temp.begin() + x, temp.begin() + n - y);
                vector<int> suffix(temp.begin() + n - y, temp.end());
                
                temp.clear();
                temp.insert(temp.end(), suffix.begin(), suffix.end());
                temp.insert(temp.end(), middle.begin(), middle.end());
                temp.insert(temp.end(), prefix.begin(), prefix.end());
                
                if (temp < best_p) {
                    best_p = temp;
                    best_x = x;
                    best_y = y;
                    improved = true;
                }
            }
        }
        
        if (improved) {
            p = best_p;
            operations.push_back({best_x, best_y});
        } else {
            break;
        }
    }
    
    cout << operations.size() << "\n";
    for (auto op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}